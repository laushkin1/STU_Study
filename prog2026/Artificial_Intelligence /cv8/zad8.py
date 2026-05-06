import os
import random
import numpy as np
import matplotlib.pyplot as plt

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader, Subset
from torchvision import datasets, transforms, models

import copy

os.environ["MPS_COPY_GRAPH_PROPERTIES"] = "1"

# ============================================================
# 1. Seed a device
# ============================================================
SEED = 42
random.seed(SEED)
np.random.seed(SEED)
torch.manual_seed(SEED)

if torch.backends.mps.is_available():
    torch.mps.manual_seed(SEED)
elif torch.cuda.is_available():
    torch.cuda.manual_seed_all(SEED)

if torch.backends.mps.is_available():
    DEVICE = torch.device("mps")
elif torch.cuda.is_available():
    DEVICE = torch.device("cuda")
else:
    DEVICE = torch.device("cpu")

print("DEVICE:", DEVICE)

# ============================================================
# 2. Filtrácia a Načítanie dát (Food101)
# ============================================================
class FilteredFood101(Dataset):
    def __init__(self, dataset, target_class_names):
        self.dataset = dataset
        self.target_class_names = target_class_names
        self.target_idx_to_new_idx = {dataset.class_to_idx[name]: i for i, name in enumerate(target_class_names)}
        
        self.all_labels = np.array(dataset._labels)
        self.valid_indices = np.where(np.isin(self.all_labels, list(self.target_idx_to_new_idx.keys())))[0]
        self.targets = np.array([self.target_idx_to_new_idx[self.all_labels[i]] for i in self.valid_indices])

    def __len__(self):
        return len(self.valid_indices)

    def __getitem__(self, idx):
        real_idx = self.valid_indices[idx]
        img, old_label = self.dataset[real_idx]
        return img, self.target_idx_to_new_idx[old_label]


transform_base = transforms.Compose([
    transforms.Resize(256),
    transforms.CenterCrop(224),
    transforms.ToTensor(),
    # from ImageNet
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])

print("Nacitavam dataset Food101...")
full_train_dataset = datasets.Food101(root="./data", split="train", download=True, transform=transform_base)
full_test_dataset = datasets.Food101(root="./data", split="test", download=True, transform=transform_base)

selected_classes = [
    "apple_pie", "caesar_salad", "clam_chowder", "edamame", "french_fries",
    "hamburger", "hot_dog", "ice_cream", "sushi", "waffles"
]

train_dataset = FilteredFood101(full_train_dataset, selected_classes)
test_dataset = FilteredFood101(full_test_dataset, selected_classes)

# ============================================================
# 3. train/val split
# ============================================================
def make_train_val_split(y, val_ratio=0.15, seed=42):
    rng = np.random.default_rng(seed)
    train_idx, val_idx = [], []
    for c in np.unique(y):
        idx_c = np.where(y == c)[0]
        idx_c = rng.permutation(idx_c)
        n_val = int(round(len(idx_c) * val_ratio))
        val_idx.extend(idx_c[:n_val])
        train_idx.extend(idx_c[n_val:])
    return np.array(train_idx, dtype=int), np.array(val_idx, dtype=int)

VAL_SEED = 123
y_all = train_dataset.targets
train_idx, val_idx = make_train_val_split(y_all, val_ratio=0.15, seed=VAL_SEED)

final_train_dataset = Subset(train_dataset, train_idx)
final_val_dataset = Subset(train_dataset, val_idx)

print(f"Train samples: {len(final_train_dataset)}")
print(f"Val samples  : {len(final_val_dataset)}")
print(f"Test samples : {len(test_dataset)}")

# ============================================================
# 4. DataLoaders & Parametre
# ============================================================
batch_size = 32 # 64
epochs = 20
lr = 0.0001
runs = 3

train_loader = DataLoader(final_train_dataset, batch_size=batch_size, shuffle=True)
val_loader = DataLoader(final_val_dataset, batch_size=batch_size, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

# ============================================================
# 5. Modely (M1, M2, M3)
# ============================================================
class M1VGG16(nn.Module):
    def __init__(self, use_pretrained=False):
        super().__init__()
        weights = models.VGG16_Weights.DEFAULT if use_pretrained else None
        self.model = models.vgg16(weights=weights)
        # Transfer Learning
        if use_pretrained:
            for param in self.model.parameters():
                param.requires_grad = False

        # Sequential[6] (final) - Linear
        in_features = self.model.classifier[6].in_features
        self.model.classifier[6] = nn.Linear(in_features, 10)

    def forward(self, x): return self.model(x)

class M2ResNet18(nn.Module):
    def __init__(self, use_pretrained=False):
        super().__init__()
        weights = models.ResNet18_Weights.DEFAULT if use_pretrained else None
        self.model = models.resnet18(weights=weights)
        # Transfer Learning
        if use_pretrained:
            for param in self.model.parameters():
                param.requires_grad = False

        # Final fc (fully connected)
        in_features = self.model.fc.in_features
        self.model.fc = nn.Linear(in_features, 10)

    def forward(self, x): return self.model(x)

class M3MobileNet(nn.Module):
    def __init__(self, use_pretrained=False):
        super().__init__()
        weights = models.MobileNet_V2_Weights.DEFAULT if use_pretrained else None
        self.model = models.mobilenet_v2(weights=weights)
        # Transfer Learning
        if use_pretrained:
            for param in self.model.parameters():
                param.requires_grad = False

        # Classifier[1] - Linear
        in_features = self.model.classifier[1].in_features
        self.model.classifier[1] = nn.Linear(in_features, 10)

    def forward(self, x): return self.model(x)

# ============================================================
# 6. Trénovacie funkcie
# ============================================================
def train_one_epoch(model, loader, criterion, optimizer, device, is_tl=False):
    model.train()
    # Pre TL BatchNorm v eval rezim
    if is_tl:
        for module in model.modules():
            if isinstance(module, nn.BatchNorm2d): module.eval()

    total_loss, total_correct, total_count = 0.0, 0, 0
    for X_batch, y_batch in loader:
        X_batch, y_batch = X_batch.to(device), y_batch.to(device)

        optimizer.zero_grad()

        logits = model(X_batch)
        loss = criterion(logits, y_batch)

        loss.backward()
        optimizer.step()

        total_loss += loss.item() * len(X_batch)
        total_correct += (logits.argmax(dim=1) == y_batch).sum().item()
        total_count += len(X_batch)

    avg_loss = total_loss / total_count
    avg_acc = total_correct / total_count
    return avg_loss, avg_acc

def evaluate(model, loader, criterion, device):
    model.eval()

    total_loss, total_correct, total_count = 0.0, 0, 0
    with torch.no_grad():
        for X_batch, y_batch in loader:
            X_batch, y_batch = X_batch.to(device), y_batch.to(device)

            logits = model(X_batch)
            loss = criterion(logits, y_batch)

            total_loss += loss.item() * len(X_batch)
            total_correct += (logits.argmax(dim=1) == y_batch).sum().item()
            total_count += len(X_batch)

    avg_loss = total_loss / total_count
    avg_acc = total_correct / total_count
    return avg_loss, avg_acc

def train_model(model_class, train_loader, val_loader, device, epochs, lr, model_params={}):
    model = model_class(**model_params).to(device)
    criterion = nn.CrossEntropyLoss()
    is_tl = model_params.get('use_pretrained', False)
    
    # Optimizer berie len vrstvy, ktore sa mozu ucit!
    trainable_params = filter(lambda p: p.requires_grad, model.parameters())
    optimizer = optim.Adam(trainable_params, lr=lr)

    history = {'train_loss': [], 'val_loss': [], 'train_acc': [], 'val_acc': []}

    # EARLY STOPPING setup
    best_val_loss = float('inf')
    patience_counter, patience = 0, 5
    best_model_weights = copy.deepcopy(model.state_dict())

    best_epoch = 1

    for epoch in range(1, epochs + 1):
        t_loss, t_acc = train_one_epoch(model, train_loader, criterion, optimizer, device, is_tl)
        v_loss, v_acc = evaluate(model, val_loader, criterion, device)
        
        history['train_loss'].append(t_loss)
        history['val_loss'].append(v_loss)
        history['train_acc'].append(t_acc)
        history['val_acc'].append(v_acc)

        # EARLY STOPPING
        if v_loss < best_val_loss:
            best_val_loss = v_loss
            patience_counter = 0
            best_model_weights = copy.deepcopy(model.state_dict())
            best_epoch = epoch
        else:
            patience_counter += 1
            if patience_counter >= patience: break

    model.load_state_dict(best_model_weights)
    return model, history, best_epoch, best_val_loss

# ============================================================
# 7. Experiment Runner
# ============================================================
GLOBAL_STATS = {}

def run_full_experiment(model_class, model_id, arch_name, mode, runs, loader_train=train_loader):
    print(f"\n[{model_id} - {arch_name} | Režim: {mode}] Trénovanie {runs} behov...")
    use_tl = (mode == "TL")
    is_aug = (mode == "TL_AUG")
    
    test_accs, test_losses = [], []
    train_accs, train_losses = [], []
    val_accs, val_losses = [], []
    best_epochs = []
    
    best_overall_model = None
    best_overall_acc = 0
    best_history = None
    criterion = nn.CrossEntropyLoss()

    run_results = []

    for i in range(runs):
        current_seed = SEED + i
        torch.manual_seed(current_seed)
        if torch.cuda.is_available(): torch.cuda.manual_seed_all(current_seed)
        if torch.backends.mps.is_available(): torch.mps.manual_seed(current_seed)

        model, history, best_epoch, best_val_loss = train_model(
            model_class, loader_train, val_loader, DEVICE, epochs, lr, model_params={'use_pretrained': use_tl or is_aug}
        )
        test_loss, test_acc = evaluate(model, test_loader, criterion, DEVICE)

        tr_l = history['train_loss'][best_epoch - 1]
        tr_a = history['train_acc'][best_epoch - 1]
        vl_l = history['val_loss'][best_epoch - 1]
        vl_a = history['val_acc'][best_epoch - 1]

        train_losses.append(tr_l); train_accs.append(tr_a * 100)
        val_losses.append(vl_l); val_accs.append(vl_a * 100)
        test_losses.append(test_loss); test_accs.append(test_acc * 100)
        best_epochs.append(best_epoch)
        
        run_results.append([i+1, tr_l, tr_a*100, vl_l, vl_a*100, test_loss, test_acc*100])

        if test_acc > best_overall_acc:
            best_overall_acc = test_acc
            best_overall_model = copy.deepcopy(model)
            best_history = history

    stats = {
        'runs': run_results,
        'mean_tr_l': np.mean(train_losses), 'mean_tr_a': np.mean(train_accs),
        'mean_vl_l': np.mean(val_losses), 'mean_vl_a': np.mean(val_accs),
        'mean_ts_l': np.mean(test_losses), 'mean_ts_a': np.mean(test_accs),
        'mean_epoch': np.mean(best_epochs)
    }
    
    GLOBAL_STATS[f"{model_id}_{mode}"] = stats
    return best_overall_model, best_history, stats

# ============================================================
# 8. SPÚŠŤANIE EXPERIMENTOV
# ============================================================
print("\n" + "="*50 + "\nZACIATOK EXPERIMENTOV\n" + "="*50)

# M1: VGG16
print("\n" + "#"*50)
print("=== M1: VGG16 ===")
print("#"*50)
vgg_s_mod, vgg_s_hist, _ = run_full_experiment(M1VGG16, "M1", "VGG16", "scratch", runs)
vgg_tl_mod, vgg_tl_hist, _ = run_full_experiment(M1VGG16, "M1", "VGG16", "TL", runs)

# M2: ResNet18
print("\n" + "#"*50)
print("=== M2: ResNet18 ===")
print("#"*50)
res_s_mod, res_s_hist, _ = run_full_experiment(M2ResNet18, "M2", "ResNet18", "scratch", runs)
res_tl_mod, res_tl_hist, _ = run_full_experiment(M2ResNet18, "M2", "ResNet18", "TL", runs)

# M3: MobileNet
print("\n" + "#"*50)
print("=== M3: MobileNet ===")
print("#"*50)
mob_s_mod, mob_s_hist, _ = run_full_experiment(M3MobileNet, "M3", "MobileNet", "scratch", runs)
mob_tl_mod, mob_tl_hist, _ = run_full_experiment(M3MobileNet, "M3", "MobileNet", "TL", runs)

# EXPERIMENT S AUGMENTACIOU (Pre najlepsi model TL)
print("\n[Pripravujem data pre Augmentaciu...]")
transform_aug = transforms.Compose([
    transforms.RandomResizedCrop(224, scale=(0.6, 1.0)),
    transforms.RandomRotation(30),
    transforms.ColorJitter(brightness=0.2, contrast=0.2, saturation=0.3, hue=0.05),
    transforms.ToTensor(),
    # from ImageNet
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])
full_train_dataset_aug = datasets.Food101(root="./data", split="train", download=False, transform=transform_aug)
train_dataset_aug = FilteredFood101(full_train_dataset_aug, selected_classes)
final_train_dataset_aug = Subset(train_dataset_aug, train_idx)
train_loader_aug = DataLoader(final_train_dataset_aug, batch_size=batch_size, shuffle=True)

# Spustenie najlepsieho modelu s augmentaciou
mob_aug_mod, mob_aug_hist, _ = run_full_experiment(M3MobileNet, "M3", "MobileNet", "TL_AUG", runs, loader_train=train_loader_aug)


# ============================================================
# 9. GENEROVANIE TABULIEK PRE REPORT (Podľa zadania v PDF)
# ============================================================
print("\n\n" + "#"*70)
print("=== VÝSLEDNÉ TABUĽKY PRE DOKUMENTÁCIU (SKOPÍRUJ DO REPORTU) ===")
print("#"*70)

# Tab 1
print("\nTabuľka 1: Zvolené modely a hlavné hyperparametre.")
print("Model\tArchitektúra\tEpochy\tLR\tBatch")
print(f"M1\tVGG16\t\t{epochs}\t{lr}\t{batch_size}")
print(f"M2\tResNet18\t{epochs}\t{lr}\t{batch_size}")
print(f"M3\tMobileNet\t{epochs}\t{lr}\t{batch_size}")

def print_run_table(table_num, desc, stats):
    print(f"\nTabuľka {table_num}: {desc}")
    print("Beh\tTrain loss\tTrain acc [%]\tVal loss\tVal acc [%]\tTest loss\tTest acc [%]")
    for r in stats['runs']:
        print(f"{r[0]}\t{r[1]:.3f}\t\t{r[2]:.1f}\t\t{r[3]:.3f}\t\t{r[4]:.1f}\t\t{r[5]:.3f}\t\t{r[6]:.1f}")

# Tab 2 & 3 (Pre M1)
print_run_table("2", "Výsledky 3 behov pre model M1 pri trénovaní od nuly (from scratch).", GLOBAL_STATS["M1_scratch"])
print_run_table("2.1", "Výsledky 3 behov pre model M2 pri trénovaní od nuly (from scratch).", GLOBAL_STATS["M2_scratch"])
print_run_table("2.2", "Výsledky 3 behov pre model M3 pri trénovaní od nuly (from scratch).", GLOBAL_STATS["M3_scratch"])
print_run_table("3", "Výsledky 3 behov pre model M1 pri transfer learningu (TL).", GLOBAL_STATS["M1_TL"])
print_run_table("3.1", "Výsledky 3 behov pre model M2 pri transfer learningu (TL).", GLOBAL_STATS["M2_TL"])
print_run_table("3.2", "Výsledky 3 behov pre model M3 pri transfer learningu (TL).", GLOBAL_STATS["M3_TL"])

# Tab 4
print(f"\nTabuľka 4: Súhrnné porovnanie režimov trénovania od nuly (from scratch) a transfer learningu (TL) pre model M1.")
print("Režim\tPriemer train loss\tPriemer train acc [%]\tPriemer val loss\tPriemer val acc [%]\tPriemer test loss\tPriemer test acc [%]")
s_sc = GLOBAL_STATS["M1_scratch"]
s_tl = GLOBAL_STATS["M1_TL"]
print(f"scratch\t{s_sc['mean_tr_l']:.3f}\t\t\t{s_sc['mean_tr_a']:.1f}\t\t\t{s_sc['mean_vl_l']:.3f}\t\t\t{s_sc['mean_vl_a']:.1f}\t\t\t{s_sc['mean_ts_l']:.3f}\t\t\t{s_sc['mean_ts_a']:.1f}")
print(f"TL\t{s_tl['mean_tr_l']:.3f}\t\t\t{s_tl['mean_tr_a']:.1f}\t\t\t{s_tl['mean_vl_l']:.3f}\t\t\t{s_tl['mean_vl_a']:.1f}\t\t\t{s_tl['mean_ts_l']:.3f}\t\t\t{s_tl['mean_ts_a']:.1f}")

# Tab 5
print("\nTabuľka 5: Súhrnné porovnanie priemerných výsledkov modelov M1, M2, M3 pri trénovaní od nuly (from scratch).")
print("Model\tArchitektúra\tPriemer test loss od nuly\tPriemer test acc [%] od nuly\tSplnenie podmienky 90%")
for m_id, arch in zip(["M1", "M2", "M3"], ["VGG16", "ResNet18", "MobileNet"]):
    s = GLOBAL_STATS[f"{m_id}_scratch"]
    splna = "áno" if s['mean_ts_a'] >= 90.0 else "nie"
    print(f"{m_id}\t{arch}\t\t{s['mean_ts_l']:.3f}\t\t\t\t{s['mean_ts_a']:.1f}\t\t\t\t{splna}")

# Tab 6
print("\nTabuľka 6: Súhrnné porovnanie priemerných výsledkov modelov M1, M2, M3 pri transfer learningu (TL).")
print("Model\tArchitektúra\tPriemer test loss TL\tPriemer test acc [%] TL\tSplnenie podmienky 93%")
for m_id, arch in zip(["M1", "M2", "M3"], ["VGG16", "ResNet18", "MobileNet"]):
    s = GLOBAL_STATS[f"{m_id}_TL"]
    splna = "áno" if s['mean_ts_a'] >= 93.0 else "nie"
    print(f"{m_id}\t{arch}\t\t{s['mean_ts_l']:.3f}\t\t\t{s['mean_ts_a']:.1f}\t\t\t{splna}")

# Tab 7
print_run_table("7", "Výsledky 3 behov pre najlepší model (M3) po doplnení augmentácie.", GLOBAL_STATS["M3_TL_AUG"])

# Tab 8
print("\nTabuľka 8: Porovnanie najlepšieho modelu (M3 MobileNet) bez augmentácie a s augmentáciou.")
print("Varianta\tVal loss\tVal acc [%]\tTest loss\tTest acc [%]\tPriemer epochy začiatku pretrénovania")
s_tl_bez = GLOBAL_STATS["M3_TL"]
s_tl_aug = GLOBAL_STATS["M3_TL_AUG"]
print(f"bez aug.\t{s_tl_bez['mean_vl_l']:.3f}\t\t{s_tl_bez['mean_vl_a']:.1f}\t\t{s_tl_bez['mean_ts_l']:.3f}\t\t{s_tl_bez['mean_ts_a']:.1f}\t\t{s_tl_bez['mean_epoch']:.1f}")
print(f"s aug.\t\t{s_tl_aug['mean_vl_l']:.3f}\t\t{s_tl_aug['mean_vl_a']:.1f}\t\t{s_tl_aug['mean_ts_l']:.3f}\t\t{s_tl_aug['mean_ts_a']:.1f}\t\t{s_tl_aug['mean_epoch']:.1f}")

# ============================================================
# 10. Grafy a Vizualizácie
# ============================================================
os.makedirs("img", exist_ok=True)


def plot_training_history(history, model_name="Model", filename="plot.png"):
    epochs_range = range(1, len(history['train_loss']) + 1)
    plt.figure(figsize=(14, 5))

    plt.subplot(1, 2, 1)
    plt.plot(epochs_range, history['train_loss'], 'b-', linewidth=2, label='Train Loss')
    plt.plot(epochs_range, history['val_loss'], 'r--', linewidth=2, label='Val Loss')
    plt.title(f'Priebeh chyby: {model_name}', fontsize=14)
    plt.xlabel('Epocha', fontsize=12)
    plt.ylabel('Loss (CrossEntropy)', fontsize=12)
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.subplot(1, 2, 2)
    plt.plot(epochs_range, history['train_acc'], 'b-', linewidth=2, label='Train Acc')
    plt.plot(epochs_range, history['val_acc'], 'r--', linewidth=2, label='Val Acc')
    plt.title(f'Priebeh úspešnosti: {model_name}', fontsize=14)
    plt.xlabel('Epocha', fontsize=12)
    plt.ylabel('Accuracy', fontsize=12)
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig(f"img/{filename}")
    plt.show()


def visualize_best_results(model, dataset, device, class_names, n_images=5, filename="results.png"):
    model.eval()
    fig, axes = plt.subplots(1, n_images, figsize=(18, 4))
    indices = random.sample(range(len(dataset)), n_images)

    mean = np.array([0.485, 0.456, 0.406])
    std = np.array([0.229, 0.224, 0.225])

    with torch.no_grad():
        for i, idx in enumerate(indices):
            img, label = dataset[idx]
            input_tensor = img.unsqueeze(0).to(device)
            output = model(input_tensor)

            pred_class = output.argmax(dim=1).item()
            pred_prob = torch.nn.functional.softmax(output, dim=1)[0][pred_class].item()

            ax = axes[i]
            img_for_plot = img.permute(1, 2, 0).numpy()
            img_for_plot = std * img_for_plot + mean  # Denormalizácia!
            img_for_plot = np.clip(img_for_plot, 0, 1)

            ax.imshow(img_for_plot)
            color = 'green' if pred_class == label else 'red'
            ax.set_title(f"Real: {class_names[label]}\nPred: {class_names[pred_class]}\n({pred_prob * 100:.1f}%)",
                         color=color)
            ax.axis('off')

    plt.tight_layout()
    # Сохраняем картинку ДО вызова plt.show()
    plt.savefig(f"img/{filename}")
    plt.show()


# Zobrazenie grafov a obrázkov pre najlepší model (с указанием имен файлов)
print("\n[Zobrazujem grafy a obrazky pre najlepsi model...]")
plot_training_history(mob_tl_hist, "M3: MobileNet (TL bez aug.)", filename="M3_TL_bez_aug_history.png")
plot_training_history(mob_aug_hist, "M3: MobileNet (TL s augmentaciou)", filename="M3_TL_s_aug_history.png")

visualize_best_results(mob_tl_mod, test_dataset, DEVICE, selected_classes, filename="M3_TL_bez_aug_viz.png")
visualize_best_results(mob_aug_mod, test_dataset, DEVICE, selected_classes, filename="M3_TL_s_aug_viz.png")
