import os
import random
import numpy as np
import matplotlib.pyplot as plt

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, Subset
from torchvision import datasets, transforms

import time
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
# 2. Načítanie dát
# ============================================================

transform = transforms.ToTensor()
train_dataset = datasets.MNIST(root="./data", train=True, download=True, transform=transform)
test_dataset = datasets.MNIST(root="./data", train=False, download=True, transform=transform)


# ============================================================
# 3. train/val split
# ============================================================

def make_train_val_split(y, excluded_idx, val_ratio, seed=42):
    rng = np.random.default_rng(seed)

    all_idx = np.arange(len(y))
    remaining_idx = np.setdiff1d(all_idx, excluded_idx, assume_unique=False)

    train_idx = []
    val_idx = []

    for c in np.unique(y):
        idx_c = remaining_idx[y[remaining_idx] == c]
        idx_c = np.sort(idx_c)
        idx_c = rng.permutation(idx_c)

        n_val = int(round(len(idx_c) * val_ratio))

        val_idx.extend(idx_c[:n_val])
        train_idx.extend(idx_c[n_val:])

    train_idx = np.array(train_idx, dtype=int)
    val_idx = np.array(val_idx, dtype=int)

    return train_idx, val_idx

VAL_SEED = 123

y_all = train_dataset.targets.numpy()

train_idx, val_idx = make_train_val_split(y_all, excluded_idx=[], val_ratio=(10000/60000), seed=VAL_SEED)

print("train samples:", len(train_idx))
print("val samples  :", len(val_idx))
print("test samples :", len(test_dataset))


# ============================================================
# 4. Dataset a DataLoader
# ============================================================

batch_size = 128

train_loader = DataLoader(Subset(train_dataset, train_idx), batch_size=batch_size, shuffle=True)
val_loader = DataLoader(Subset(train_dataset, val_idx), batch_size=batch_size, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

# ============================================================
# 5. Parametre
# ============================================================

epochs = 15
lr = 0.001 # Learning Rate
runs = 5

# ============================================================
# 6. Model
# ============================================================

class MLP1(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 128),
            nn.ReLU(),
            nn.Linear(128, 10)
        )
    def forward(self, x): return self.net(x)

class MLP2(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 256),
            nn.ReLU(),
            nn.Linear(256, 128),
            nn.ReLU(),
            nn.Linear(128, 10)
        )
    def forward(self, x): return self.net(x)


class CNN1(nn.Module):
    def __init__(self):
        super().__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
             nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2)
        )
        self.classifier = nn.Sequential(
            nn.Linear(64 * 7 * 7, 128),
            nn.ReLU(),
            nn.Linear(128, 10)
        )
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        return self.classifier(x)

class CNN2(nn.Module):
    def __init__(self):
        super().__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2)
        )
        self.classifier = nn.Sequential(
            nn.Linear(128 * 3 * 3, 128),
            nn.ReLU(),
            nn.Linear(128, 10)
        )
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        return self.classifier(x)

class CNN3(nn.Module):
    def __init__(self):
        super().__init__()
        self.features = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2)
        )
        self.classifier = nn.Sequential(
            nn.Linear(128 * 3 * 3, 256),
            nn.ReLU(),
            nn.Linear(256, 10)
        )
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        return self.classifier(x)


class MLP_Dropout(nn.Module):
    def __init__(self, p):
        super().__init__()
        self.net = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 256),
            nn.ReLU(),
            nn.Dropout(p),
            nn.Linear(256, 128),
            nn.ReLU(),
            nn.Dropout(p),
            nn.Linear(128, 10)
        )
    def forward(self, x):
        return self.net(x)


# ============================================================
# 7. Train a eval slučky
# ============================================================

def train_one_epoch(model, loader, criterion, optimizer, device):
    model.train()

    total_loss = 0.0
    total_correct = 0
    total_count = 0

    for X_batch, y_batch in loader:
        X_batch = X_batch.to(device)
        y_batch = y_batch.to(device)

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

    total_loss = 0.0
    total_correct = 0
    total_count = 0

    with torch.no_grad():
        for X_batch, y_batch in loader:
            X_batch = X_batch.to(device)
            y_batch = y_batch.to(device)

            logits = model(X_batch)
            loss = criterion(logits, y_batch)

            total_loss += loss.item() * len(X_batch)
            total_correct += (logits.argmax(dim=1) == y_batch).sum().item()
            total_count += len(X_batch)

    avg_loss = total_loss / total_count
    avg_acc = total_correct / total_count
    return avg_loss, avg_acc

# ============================================================
# 8. Tréning
# ============================================================

def train_model(model_class, train_loader, val_loader, device, epochs, lr, model_params={}):
    model = model_class(**model_params).to(device)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=lr)

    history = {'train_loss': [], 'val_loss': [], 'train_acc': [], 'val_acc': []}

    # EARLY STOPPING setup
    best_val_loss = float('inf')
    patience_counter = 0
    patience = 5
    best_model_weights = copy.deepcopy(model.state_dict())

    best_epoch = 1

    for epoch in range(1, epochs + 1):
        t_loss, t_acc = train_one_epoch(model, train_loader, criterion, optimizer, device)
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
            if patience_counter >= patience:
                break

    model.load_state_dict(best_model_weights)
    return model, history, best_epoch, best_val_loss


# ============================================================
# 9. Tréningový experiment
# ============================================================

def run_full_experiment(model_class, name, runs, model_params={}):
    print(f"\n" + "="*70)
    print(f"EXPERIMENT: {name}")
    print("="*70)


    test_accs = []
    test_losses = []
    train_accs = []
    val_losses = []
    best_epochs = []

    best_overall_model = None
    best_overall_acc = 0
    best_history = None

    criterion = nn.CrossEntropyLoss()

    print("Beh\tTrain loss\tTest loss\tTrain acc [%]\tTest acc [%]\t(Stop Epocha)")

    for i in range(runs):
        current_seed = SEED + i
        torch.manual_seed(current_seed)
        if torch.cuda.is_available(): torch.cuda.manual_seed_all(current_seed)
        if torch.backends.mps.is_available(): torch.mps.manual_seed(current_seed)


        model, history, best_epoch, best_val_loss = train_model(
            model_class, train_loader, val_loader, DEVICE, epochs, lr, model_params=model_params
        )

        test_loss, test_acc = evaluate(model, test_loader, criterion, DEVICE)

        train_loss = history['train_loss'][best_epoch - 1]
        train_acc = history['train_acc'][best_epoch - 1]

        test_accs.append(test_acc * 100)
        test_losses.append(test_loss)
        train_accs.append(train_acc * 100)
        val_losses.append(best_val_loss)
        best_epochs.append(best_epoch)

        print(f"{i+1}\t{train_loss:.3f}\t\t{test_loss:.3f}\t\t{train_acc*100:.1f}\t\t{test_acc*100:.1f}\t\t({best_epoch})")

        if test_acc > best_overall_acc:
            best_overall_acc = test_acc
            best_overall_model = copy.deepcopy(model)
            best_history = history

    stats = {
        'min_test_acc':     np.min(test_accs),
        'max_test_acc':     np.max(test_accs),
        'mean_test_acc':    np.mean(test_accs),
        'mean_test_loss':   np.mean(test_losses),
        'mean_train_acc':   np.mean(train_accs),
        'mean_val_loss':    np.mean(val_losses),
        'mean_epoch':       np.mean(best_epochs)
    }

    print("\n--- Súhrnná štatistika pre protokol ---")
    print("Model\tMin test acc\tMax test acc\tPriemer test acc\tPriemer test loss")
    print(f"{name}\t{stats['min_test_acc']:.1f}%\t\t{stats['max_test_acc']:.1f}%\t\t{stats['mean_test_acc']:.1f}%\t\t\t{stats['mean_test_loss']:.3f}")

    if "Dropout" in name:
        print("\n--- Dáta pre Dropout Tabuľky (Tab. 10 a 11) ---")
        print(f"Priemer epochy pretrénovania: {stats['mean_epoch']:.1f}")
        print(f"Priemer val loss: {stats['mean_val_loss']:.3f}")
        print(f"Priemer train acc: {stats['mean_train_acc']:.1f}%")

    return best_overall_model, best_history, stats

mlp1_model, mlp1_hist, mlp1_stats = run_full_experiment(MLP1, "MLP1", runs)
mlp2_model, mlp2_hist, mlp2_stats = run_full_experiment(MLP2, "MLP2", runs)

cnn1_model, cnn1_hist, cnn1_stats = run_full_experiment(CNN1, "CNN1", runs)
cnn2_model, cnn2_hist, cnn2_stats = run_full_experiment(CNN2, "CNN2", runs)
cnn3_model, cnn3_hist, cnn3_stats = run_full_experiment(CNN3, "CNN3", runs)

drop1_model, _, _ = run_full_experiment(MLP_Dropout, "Dropout 0.1", runs, model_params={'p': 0.1})
drop3_model, _, _ = run_full_experiment(MLP_Dropout, "Dropout 0.3", runs, model_params={'p': 0.3})
drop5_model, _, _ = run_full_experiment(MLP_Dropout, "Dropout 0.5", runs, model_params={'p': 0.5})

# ============================================================
# 10. Porovnanie času trénovania na CPU a GPU
# ============================================================

def compare_cpu_gpu(model_class, name):
    print("\n" + "="*50)
    print(f"Porovnanie rýchlosti pre: {name}")

    criterion = nn.CrossEntropyLoss()
    cpu_time = 0
    gpu_time = 0

    # Identify the best available GPU backend
    if torch.backends.mps.is_available():
        gpu_backend = "mps"
    elif torch.cuda.is_available():
        gpu_backend = "cuda"
    else:
        gpu_backend = None

    # 1. Test na CPU
    cpu_model = model_class().to("cpu")
    optimizer_cpu = optim.Adam(cpu_model.parameters(), lr)

    start_time = time.time()
    train_one_epoch(cpu_model, train_loader, criterion, optimizer_cpu, "cpu")
    cpu_time = time.time() - start_time

    # 2. Test na GPU
    if gpu_backend:
        gpu_model = model_class().to(gpu_backend)
        optimizer_gpu = optim.Adam(gpu_model.parameters(), lr)

        start_time = time.time()
        train_one_epoch(gpu_model, train_loader, criterion, optimizer_gpu, gpu_backend)
        gpu_time = time.time() - start_time

    print("Model\tCPU čas [s]\tGPU čas [s]\tZrýchlenie")
    if gpu_time > 0:
        print(f"{name}\t{cpu_time:.1f}\t\t{gpu_time:.1f}\t\t{cpu_time/gpu_time:.1f}x")
    else:
        print(f"{name}\t{cpu_time:.1f}\t\tN/A\t\tN/A")

compare_cpu_gpu(MLP2, "MLP2")
compare_cpu_gpu(CNN2, "CNN2")

# ============================================================
# 11. Grafy
# ============================================================

def get_confusion_matrix(model, loader, device, num_classes=10):
    model.eval()

    cm = torch.zeros(num_classes, num_classes, dtype=torch.int64)

    with torch.no_grad():
        for X_batch, y_batch in loader:
            X_batch = X_batch.to(device)
            y_batch = y_batch.to(device)

            logits = model(X_batch)
            preds = logits.argmax(dim=1)


            for t, p in zip(y_batch.view(-1), preds.view(-1)):
                cm[t.long(), p.long()] += 1

    return cm.numpy()

def plot_confusion_matrix(cm, title="Kontingenčná matica"):
    plt.figure(figsize=(8, 6))

    plt.imshow(cm, interpolation='nearest', cmap=plt.cm.Blues)
    plt.title(title)
    plt.colorbar()

    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            plt.text(j, i, str(cm[i, j]), horizontalalignment="center",
                     color="white" if cm[i, j] > cm.max() / 2 else "black")

    plt.ylabel('Skutočná trieda')
    plt.xlabel('Predikovaná trieda')
    plt.tight_layout()
    plt.show()

cm_mlp1 = get_confusion_matrix(mlp1_model, test_loader, DEVICE)
cm_mlp2 = get_confusion_matrix(mlp2_model, test_loader, DEVICE)

cm_cnn1 = get_confusion_matrix(cnn1_model, test_loader, DEVICE)
cm_cnn2 = get_confusion_matrix(cnn2_model, test_loader, DEVICE)
cm_cnn3 = get_confusion_matrix(cnn3_model, test_loader, DEVICE)

cm_drop1 = get_confusion_matrix(drop1_model, test_loader, DEVICE)
cm_drop3 = get_confusion_matrix(drop3_model, test_loader, DEVICE)
cm_drop5 = get_confusion_matrix(drop5_model, test_loader, DEVICE)

plot_confusion_matrix(cm_mlp1, title="Confusion Matrix - MLP_1")
plot_confusion_matrix(cm_mlp2, title="Confusion Matrix - MLP_2")

plot_confusion_matrix(cm_cnn1, title="Confusion Matrix - CNN_1")
plot_confusion_matrix(cm_cnn2, title="Confusion Matrix - CNN_2")
plot_confusion_matrix(cm_cnn3, title="Confusion Matrix - CNN_3")

plot_confusion_matrix(cm_drop1, title="Confusion Matrix - DROPOUT 0.1")
plot_confusion_matrix(cm_drop3, title="Confusion Matrix - DROPOUT 0.3")
plot_confusion_matrix(cm_drop5, title="Confusion Matrix - DROPOUT 0.5")

# ============================================================
# 12. Vizualizácia výsledkov na testovacích dátach
# ============================================================

def visualize_10_digits(model, dataset, device):
    model.eval()
    fig, axes = plt.subplots(2, 5, figsize=(15, 7))
    fig.suptitle('Ukážka klasifikácie (Skutočná vs. Predikovaná)', fontsize=16)

    found_digits = set()

    with torch.no_grad():
        for img, label in dataset:
            if len(found_digits) == 10:
                break

            if label not in found_digits:

                input_tensor = img.unsqueeze(0).to(device)

                output = model(input_tensor)
                pred_class = output.argmax(dim=1).item()
                probs = torch.nn.functional.softmax(output, dim=1)
                pred_prob = probs[0][pred_class].item()

                row = label // 5
                col = label % 5
                ax = axes[row, col]

                ax.imshow(img.squeeze().cpu().numpy(), cmap='gray')

                color = 'green' if pred_class == label else 'red'
                ax.set_title(f"Real: {label} | Pred: {pred_class}\nProb: {pred_prob*100:.1f}%", color=color)
                ax.axis('off')

                found_digits.add(label)

    plt.tight_layout()
    plt.show()


print("\nVisualizing 10 digits for MLP_1")
visualize_10_digits(mlp1_model, test_dataset, DEVICE)
print("\nVisualizing 10 digits for MLP_2")
visualize_10_digits(mlp2_model, test_dataset, DEVICE)

print("\nVisualizing 10 digits for CNN_1")
visualize_10_digits(cnn1_model, test_dataset, DEVICE)
print("\nVisualizing 10 digits for CNN_2")
visualize_10_digits(cnn2_model, test_dataset, DEVICE)
print("\nVisualizing 10 digits for CNN_3")
visualize_10_digits(cnn3_model, test_dataset, DEVICE)

print("\nVisualizing 10 digits for Dropout 0.1...")
visualize_10_digits(drop1_model, test_dataset, DEVICE)
print("\nVisualizing 10 digits for Dropout 0.3...")
visualize_10_digits(drop3_model, test_dataset, DEVICE)
print("\nVisualizing 10 digits for Dropout 0.5...")
visualize_10_digits(drop5_model, test_dataset, DEVICE)

# ============================================================
# 13. Priebehy učenia
# ============================================================

def plot_training_history(history, model_name="Model"):

    epochs = range(1, len(history['train_loss']) + 1)

    plt.figure(figsize=(14, 5))

    plt.subplot(1, 2, 1)
    plt.plot(epochs, history['train_loss'], 'b-', linewidth=2, label='Trénovacia chyba (Train Loss)')
    plt.plot(epochs, history['val_loss'], 'r--', linewidth=2, label='Validačná chyba (Val Loss)')

    plt.title(f'Priebeh chyby počas trénovania: {model_name}', fontsize=14)
    plt.xlabel('Epocha', fontsize=12)
    plt.ylabel('Loss (CrossEntropy)', fontsize=12)
    plt.legend(fontsize=11)
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.subplot(1, 2, 2)
    plt.plot(epochs, history['train_acc'], 'b-', linewidth=2, label='Trénovacia presnosť (Train Acc)')
    plt.plot(epochs, history['val_acc'], 'r--', linewidth=2, label='Validačná presnosť (Val Acc)')

    plt.title(f'Priebeh úspešnosti klasifikácie: {model_name}', fontsize=14)
    plt.xlabel('Epocha', fontsize=12)
    plt.ylabel('Accuracy', fontsize=12)
    plt.legend(fontsize=11)
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.show()


print("Generujem grafy pre MLP1...")
plot_training_history(mlp1_hist, "MLP1 (1 Skryta vrstvy)")

print("Generujem grafy pre MLP2...")
plot_training_history(mlp2_hist, "MLP2 (2 Skryte vrstvy)")

print("Generujem grafy pre CNN1...")
plot_training_history(cnn1_hist, "CNN1 (2 Conv vrstvy)")

print("Generujem grafy pre CNN2...")
plot_training_history(cnn2_hist, "CNN2 (3 Conv vrstvy)")

print("Generujem grafy pre CNN3...")
plot_training_history(cnn3_hist, "CNN3 (3 Conv vrstvy)")
