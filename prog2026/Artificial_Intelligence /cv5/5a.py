# ============================================================
# 5a - Klasifikácia bodov pomocou MLP v PyTorch
# čistý základný kód
# ============================================================

# Ak treba:
# !pip install tensorboard pandas matplotlib plotly

import os
import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import plotly.express as px

import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader, Subset
from torch.utils.tensorboard import SummaryWriter


# ============================================================
# 1. Seed a device
# ============================================================

SEED = 42
random.seed(SEED)
np.random.seed(SEED)
torch.manual_seed(SEED)
if torch.cuda.is_available():
    torch.cuda.manual_seed_all(SEED)

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print("DEVICE:", DEVICE)


# ============================================================
# 2. Načítanie dát
# ============================================================

DATABODY_PATH = "databody.csv"
DATABODY_URL = "https://raw.githubusercontent.com/STU-FEI-OUI/UMINT-UNS_data/main/Python_(CSV)/databody.csv"


def load_csv(local_path, remote_url):
    if os.path.exists(local_path):
        return pd.read_csv(local_path)
    return pd.read_csv(remote_url)


df = load_csv(DATABODY_PATH, DATABODY_URL)

X_all = df[["x", "y", "z"]].values.astype(np.float32)
y_raw = df["label"].values

classes = np.sort(np.unique(y_raw))
class_to_idx = {c: i for i, c in enumerate(classes)}
idx_to_class = {i: c for c, i in class_to_idx.items()}

y_all = np.array([class_to_idx[v] for v in y_raw], dtype=np.int64)
num_classes = len(classes)


# ============================================================
# 3. Fixný test split + train/val split zo zvyšku
# test je vždy rovnaký: rovnaké dáta aj rovnaké poradie
# ============================================================

def make_fixed_test_split(y, test_ratio=0.15, seed=42):
    rng = np.random.default_rng(seed)

    test_idx = []

    for c in np.unique(y):
        idx_c = np.where(y == c)[0]
        idx_c = np.sort(idx_c)                 # stabilný základ
        idx_c = rng.permutation(idx_c)         # deterministický výber pri pevnom seed

        n_test = int(round(len(idx_c) * test_ratio))
        test_idx.extend(idx_c[:n_test])

    test_idx = np.array(test_idx, dtype=int)

    # zachováme pevné poradie testovacích vzoriek
    # najprv podľa tried, potom podľa deterministického výberu v triede
    return test_idx


def make_train_val_split(y, excluded_idx, val_ratio=0.15, seed=42):
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


TEST_SEED = 42
VAL_SEED = 123

test_idx = make_fixed_test_split(y_all, test_ratio=0.15, seed=TEST_SEED)
train_idx, val_idx = make_train_val_split(y_all, excluded_idx=test_idx, val_ratio=0.15, seed=VAL_SEED)

print("train samples:", len(train_idx))
print("val samples  :", len(val_idx))
print("test samples :", len(test_idx))

# ============================================================
# 4. Normalizácia vstupov podľa train množiny
# ============================================================

X_train_raw = X_all[train_idx]
x_mean = X_train_raw.mean(axis=0, keepdims=True)
x_std = X_train_raw.std(axis=0, keepdims=True) + 1e-8

X_all = (X_all - x_mean) / x_std


# ============================================================
# 5. Dataset a DataLoader
# ============================================================

class SimpleDataset(Dataset):
    def __init__(self, X, y):
        self.X = torch.tensor(X, dtype=torch.float32)
        self.y = torch.tensor(y, dtype=torch.long)

    def __len__(self):
        return len(self.X)

    def __getitem__(self, idx):
        return self.X[idx], self.y[idx]


dataset = SimpleDataset(X_all, y_all)

batch_size = 32

train_loader = DataLoader(
    Subset(dataset, train_idx),
    batch_size=batch_size,
    shuffle=True
)

val_loader = DataLoader(
    Subset(dataset, val_idx),
    batch_size=batch_size,
    shuffle=False
)

test_loader = DataLoader(
    Subset(dataset, test_idx),
    batch_size=batch_size,
    shuffle=False
)

# ============================================================
# 6. Parametre
# ============================================================

hidden_dim = 32
epochs = 200
lr = 0.0007

log_dir = "runs/5a_classification"

# ============================================================
# 7. Model
# ============================================================

class SimpleMLP(nn.Module):
    def __init__(self, input_dim, hidden_dim, output_dim):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, output_dim)
        )

    def forward(self, x):
        return self.net(x)


model = SimpleMLP(
    input_dim=3,
    hidden_dim=hidden_dim,
    output_dim=num_classes
).to(DEVICE)

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=lr)


# ============================================================
# 8. Train a eval slučky
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
# 9. Tréning
# ============================================================

writer = SummaryWriter(log_dir=log_dir)

history_train_loss = []
history_val_loss = []
history_train_acc = []
history_val_acc = []

for epoch in range(1, epochs + 1):
    train_loss, train_acc = train_one_epoch(model, train_loader, criterion, optimizer, DEVICE)
    val_loss, val_acc = evaluate(model, val_loader, criterion, DEVICE)

    history_train_loss.append(train_loss)
    history_val_loss.append(val_loss)
    history_train_acc.append(train_acc)
    history_val_acc.append(val_acc)

    writer.add_scalar("loss/train", train_loss, epoch)
    writer.add_scalar("loss/val", val_loss, epoch)
    writer.add_scalar("acc/train", train_acc, epoch)
    writer.add_scalar("acc/val", val_acc, epoch)

    if epoch == 1 or epoch % 20 == 0 or epoch == epochs:
        print(
            f"Epoch {epoch:4d}/{epochs} | "
            f"train_loss={train_loss:.8e} | "
            f"val_loss={val_loss:.8e} | "
            f"train_acc={train_acc:.4f} | "
            f"val_acc={val_acc:.4f}"
        )

writer.close()


# ============================================================
# 10. TensorBoard
# ============================================================

# Jupyter / Colab:
# %load_ext tensorboard
# %tensorboard --logdir runs


# ============================================================
# 11. Graf train_loss vs val_loss
# ============================================================

plt.figure(figsize=(9, 4.5))
plt.plot(history_train_loss, label="train_loss")
plt.plot(history_val_loss, label="val_loss")

if TARGET_LOSS is not None:
    plt.axhline(TARGET_LOSS, color="black", linestyle="--", label=f"cieľ {TARGET_LOSS:.0e}")

plt.xlabel("epoch")
plt.ylabel("loss")
plt.title("5a - train_loss a val_loss")
plt.grid(True)
plt.legend()
plt.show()


# ============================================================
# 12. Finálne test metriky
# ============================================================

test_loss, test_acc = evaluate(model, test_loader, criterion, DEVICE)

print(f"test_loss = {test_loss:.8e}")
print(f"test_acc  = {test_acc:.4f}")


# ============================================================
# 13. Confusion matrix
# ============================================================

def confusion_matrix_numpy(model, loader, device, num_classes):
    model.eval()

    y_true_all = []
    y_pred_all = []

    with torch.no_grad():
        for X_batch, y_batch in loader:
            X_batch = X_batch.to(device)

            logits = model(X_batch)
            y_pred = logits.argmax(dim=1).cpu().numpy()

            y_true_all.extend(y_batch.numpy())
            y_pred_all.extend(y_pred)

    y_true_all = np.array(y_true_all)
    y_pred_all = np.array(y_pred_all)

    cm = np.zeros((num_classes, num_classes), dtype=int)
    for t, p in zip(y_true_all, y_pred_all):
        cm[t, p] += 1

    return cm


cm = confusion_matrix_numpy(model, test_loader, DEVICE, num_classes)

plt.figure(figsize=(6, 5))
plt.imshow(cm, cmap="Blues")
plt.title("Confusion matrix - test")
plt.xlabel("Predicted class")
plt.ylabel("True class")
plt.xticks(range(num_classes), [idx_to_class[i] for i in range(num_classes)])
plt.yticks(range(num_classes), [idx_to_class[i] for i in range(num_classes)])

for i in range(num_classes):
    for j in range(num_classes):
        plt.text(j, i, cm[i, j], ha="center", va="center")

plt.colorbar()
plt.tight_layout()
plt.show()


# ============================================================
# 14. 3D graf: train+val ako body, test ako x
# farba určuje triedu
# ============================================================

import plotly.graph_objects as go
import plotly.express as px

X_raw = df[["x", "y", "z"]].values.astype(np.float32)

train_val_idx = np.concatenate([train_idx, val_idx])
X_train_val_raw = X_raw[train_val_idx]
y_train_val_raw = y_raw[train_val_idx]

X_test_raw = X_raw[test_idx]
y_test_raw = y_raw[test_idx]

# pevné farby tried
palette = px.colors.qualitative.Plotly
class_colors = {cls: palette[i % len(palette)] for i, cls in enumerate(classes)}

fig = go.Figure()

for cls in classes:
    color = class_colors[cls]

    mask_tv = (y_train_val_raw == cls)
    mask_test = (y_test_raw == cls)

    # train+val body - legenda len tu
    fig.add_trace(go.Scatter3d(
        x=X_train_val_raw[mask_tv, 0],
        y=X_train_val_raw[mask_tv, 1],
        z=X_train_val_raw[mask_tv, 2],
        mode="markers",
        name=f"trieda {cls}",
        marker=dict(
            size=4,
            symbol="circle",
            color=color
        ),
        showlegend=True
    ))

    # test body - rovnaká farba, bez legendy
    fig.add_trace(go.Scatter3d(
        x=X_test_raw[mask_test, 0],
        y=X_test_raw[mask_test, 1],
        z=X_test_raw[mask_test, 2],
        mode="markers",
        marker=dict(
            size=7,
            symbol="x",
            color=color
        ),
        showlegend=False
    ))

fig.update_layout(
    title="5a - train+val body, test = x",
    scene=dict(
        xaxis_title="x",
        yaxis_title="y",
        zaxis_title="z"
    ),
    legend_title="Trieda"
)

fig.show()


# ============================================================
# 15. Vytvorenie 5 náhodných bodov v rozsahu skupín + inferencia
# pre každú triedu vytvoríme 1 bod v min/max rozsahu danej triedy
# ============================================================

rng = np.random.default_rng(SEED)

random_points_raw = []
random_points_expected_class = []

for cls in classes:
    class_points = X_raw[y_raw == cls]

    mins = class_points.min(axis=0)
    maxs = class_points.max(axis=0)

    point = rng.uniform(low=mins, high=maxs).astype(np.float32)

    random_points_raw.append(point)
    random_points_expected_class.append(cls)

random_points_raw = np.array(random_points_raw, dtype=np.float32)

# normalizácia podľa train štatistík
random_points_scaled = (random_points_raw - x_mean) / x_std

random_tensor = torch.tensor(random_points_scaled, dtype=torch.float32, device=DEVICE)

model.eval()
with torch.no_grad():
    random_logits = model(random_tensor)
    random_pred_idx = torch.argmax(random_logits, dim=1).cpu().numpy()

random_pred_class = np.array([idx_to_class[i] for i in random_pred_idx])

print("5 náhodných bodov a ich klasifikácia:")
for i in range(len(random_points_raw)):
    p = random_points_raw[i]
    print(
        f"bod {i+1}: "
        f"x={p[0]:.4f}, y={p[1]:.4f}, z={p[2]:.4f} | "
        f"rozsah triedy={random_points_expected_class[i]} | "
        f"predikovaná trieda={random_pred_class[i]}"
    )


# ============================================================
# 16. 3D graf: všetky pôvodné body ako body
# a našich 5 náhodných bodov ako x
# farba určuje triedu
# ============================================================

fig = go.Figure()

for cls in classes:
    color = class_colors[cls]

    mask_all = (y_raw == cls)
    mask_random = (random_pred_class == cls)

    # pôvodné dáta - legenda len tu
    fig.add_trace(go.Scatter3d(
        x=X_raw[mask_all, 0],
        y=X_raw[mask_all, 1],
        z=X_raw[mask_all, 2],
        mode="markers",
        name=f"trieda {cls}",
        marker=dict(
            size=4,
            symbol="circle",
            color=color
        ),
        showlegend=True
    ))

    # náhodné body - rovnaká farba podľa predikovanej triedy, bez legendy
    fig.add_trace(go.Scatter3d(
        x=random_points_raw[mask_random, 0],
        y=random_points_raw[mask_random, 1],
        z=random_points_raw[mask_random, 2],
        mode="markers",
        marker=dict(
            size=9,
            symbol="x",
            color=color
        ),
        showlegend=False
    ))

fig.update_layout(
    title="5a - pôvodné body, náhodné body = x",
    scene=dict(
        xaxis_title="x",
        yaxis_title="y",
        zaxis_title="z"
    ),
    legend_title="Trieda"
)

fig.show()