import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import confusion_matrix

# Load Data
file = "CTGdata.csv"
df = pd.read_csv(file)

# First 25 columns are features, the last is the target class
X_all = df.iloc[:, :25].values.astype(np.float32)
y_raw = df['typ_ochorenia'].values

# PyTorch expects classes from 0 to C-1. We have 1, 2, 3, so we subtract 1.
y_all = (y_raw - 1).astype(np.int64)
num_classes = 3

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Device in use: {DEVICE}\n")


# Define 3 Network Structures (M1, M2, M3)

class MLP_Model1(nn.Module):
    # M1: 1 hidden layer (10 neurons)
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(25, 10), nn.ReLU(),
            nn.Linear(10, num_classes)
        )

    def forward(self, x): return self.net(x)


class MLP_Model2(nn.Module):
    # M2: 1 hidden layer (20 neurons)
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(25, 20), nn.ReLU(),
            nn.Linear(20, num_classes)
        )

    def forward(self, x): return self.net(x)


class MLP_Model3(nn.Module):
    # M3: 2 hidden layers (20 and 10 neurons)
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(25, 20), nn.ReLU(),
            nn.Linear(20, 10), nn.ReLU(),
            nn.Linear(10, num_classes)
        )

    def forward(self, x): return self.net(x)


# Hyperparameters
EPOCHS = 150
LR = 0.001
BATCH_SIZE = 32
NUM_RUNS = 5

models_dict = {
    "M1 (10)": MLP_Model1,
    "M2 (20)": MLP_Model2,
    "M3 (20,10)": MLP_Model3
}


class CTGDataset(Dataset):
    def __init__(self, X, y):
        self.X = torch.tensor(X, dtype=torch.float32)
        self.y = torch.tensor(y, dtype=torch.long)

    def __len__(self): return len(self.X)

    def __getitem__(self, idx): return self.X[idx], self.y[idx]


def train_and_evaluate(model_class, seed):
    # Fix seed for reproducibility of this specific run
    torch.manual_seed(seed)
    np.random.seed(seed)

    # 60% Train, 20% Val, 20% Test
    X_temp, X_test, y_temp, y_test = train_test_split(X_all, y_all, test_size=0.20, random_state=seed, stratify=y_all)
    X_train, X_val, y_train, y_val = train_test_split(X_temp, y_temp, test_size=0.25, random_state=seed, stratify=y_temp)

    # Data Normalization Train set
    scaler = StandardScaler()
    X_train = scaler.fit_transform(X_train)
    X_val = scaler.transform(X_val)
    X_test = scaler.transform(X_test)

    train_loader = DataLoader(CTGDataset(X_train, y_train), batch_size=BATCH_SIZE, shuffle=True)
    val_loader = DataLoader(CTGDataset(X_val, y_val), batch_size=BATCH_SIZE, shuffle=False)
    test_loader = DataLoader(CTGDataset(X_test, y_test), batch_size=BATCH_SIZE, shuffle=False)

    model = model_class().to(DEVICE)
    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=LR)

    train_losses, val_losses = [], []

    # Training Loop
    for epoch in range(EPOCHS):
        model.train()
        epoch_train_loss = 0
        for X_batch, y_batch in train_loader:
            optimizer.zero_grad()
            out = model(X_batch.to(DEVICE))
            loss = criterion(out, y_batch.to(DEVICE))
            loss.backward()
            optimizer.step()
            epoch_train_loss += loss.item() * len(X_batch)
        train_losses.append(epoch_train_loss / len(X_train))

        # Validation Loop
        model.eval()
        epoch_val_loss = 0
        with torch.no_grad():
            for X_batch, y_batch in val_loader:
                out = model(X_batch.to(DEVICE))
                loss = criterion(out, y_batch.to(DEVICE))
                epoch_val_loss += loss.item() * len(X_batch)
        val_losses.append(epoch_val_loss / len(X_val))

    # Evaluation on Test Set
    model.eval()
    correct, total = 0, 0
    all_preds, all_targets = [], []
    with torch.no_grad():
        for X_batch, y_batch in test_loader:
            out = model(X_batch.to(DEVICE))
            preds = out.argmax(dim=1).cpu().numpy()
            all_preds.extend(preds)
            all_targets.extend(y_batch.numpy())
            correct += (preds == y_batch.numpy()).sum()
            total += len(y_batch)

    test_acc = correct / total
    return test_acc, all_targets, all_preds, model, train_losses, val_losses, scaler, X_test



# Run Experiments
best_overall_acc = 0
best_model_info = None

print("Starting training. Please wait...\n")

for name, m_class in models_dict.items():
    print(f"--- Testing Model Structure: {name} ---")
    acc_list = []
    best_run_acc = 0
    best_run_train_losses = []
    best_run_val_losses = []

    for run in range(NUM_RUNS):
        acc, targets, preds, model, t_losses, v_losses, scaler, X_test = train_and_evaluate(m_class, seed=42 + run)
        acc_list.append(acc * 100)

        # Track the best run FOR THIS SPECIFIC MODEL (for plotting)
        if acc > best_run_acc:
            best_run_acc = acc
            best_run_train_losses = t_losses
            best_run_val_losses = v_losses

        # Track the best overall model across ALL structures
        if acc > best_overall_acc:
            best_overall_acc = acc
            best_model_info = {
                "name": name, "model": model, "targets": targets,
                "preds": preds, "scaler": scaler, "X_test": X_test
            }

    print(f"Results from 5 runs: {[f'{a:.2f}%' for a in acc_list]}")
    print(f"Min: {np.min(acc_list):.2f}% | Max: {np.max(acc_list):.2f}% | Average: {np.mean(acc_list):.2f}%\n")

    # Plot Loss Curve for the best run of this model structure
    plt.figure(figsize=(8, 4))
    plt.plot(best_run_train_losses, label='Train Loss')
    plt.plot(best_run_val_losses, label='Validation Loss')
    plt.title(f"Learning Curve for {name} (Best Run)")
    plt.xlabel("Epochs")
    plt.ylabel("Loss")
    plt.legend()
    plt.grid(True)
    plt.show()


# Analysis of the Best Network (Metrics)
print("="*54)
print(f"Best Overall Network: {best_model_info['name']} with Accuracy: {best_overall_acc * 100:.2f}%")

cm = confusion_matrix(best_model_info['targets'], best_model_info['preds'])
print("\nConfusion Matrix:")
print(cm)

class_names = ["Normal (normálny)", "Suspect (podozrivý)", "Pathologic (patologický)"]

for i in range(num_classes):
    TP = cm[i, i]
    FP = cm[:, i].sum() - TP
    FN = cm[i, :].sum() - TP
    TN = cm.sum() - (TP + FP + FN)

    sensitivity = TP / (TP + FN) if (TP + FN) > 0 else 0
    specificity = TN / (TN + FP) if (TN + FP) > 0 else 0

    print(f"\nClass {i + 1}: {class_names[i]}:")
    print(f"  Sensitivity: {sensitivity * 100:.2f}%")
    print(f"  Specificity: {specificity * 100:.2f}%")


# Testing 1 sample of each type
print("\n--- Testing individual samples from the dataset ---")
best_model = best_model_info['model']
best_model.eval()
X_test = best_model_info['X_test']
y_test = best_model_info['targets']

tested_classes = set()
with torch.no_grad():
    for i in range(len(y_test)):
        true_class = y_test[i]
        if true_class not in tested_classes:
            sample = torch.tensor(X_test[i], dtype=torch.float32).unsqueeze(0).to(DEVICE)
            pred = best_model(sample).argmax(dim=1).item()
            print(f"Sample {i}: True Class = {true_class + 1} ({class_names[true_class]}), "
                  f"Predicted Class = {pred + 1} ({class_names[pred]})")
            tested_classes.add(true_class)
        if len(tested_classes) == 3:
            break