import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

# 1. Data Loading and Normalization
data = pd.read_csv('datafun.csv', header=None).values.astype(np.float32)
x_raw = data[:, 0].reshape(-1, 1)
y_raw = data[:, 1].reshape(-1, 1)

# We scale x and y to the range [-1, 1].
scaler_x = StandardScaler()
scaler_y = StandardScaler()
x_scaled = scaler_x.fit_transform(x_raw)
y_scaled = scaler_y.fit_transform(y_raw)


# 2. Model (25 neurons)
class BetterApproximation(nn.Module):
    def __init__(self):
        super(BetterApproximation, self).__init__()
        self.net = nn.Sequential(
            nn.Linear(1, 25),
            nn.Tanh(),  # Using the tanh function for smoothness
            nn.Linear(25, 25),  # Add another layer for flexibility
            nn.Tanh(),
            nn.Linear(25, 1)
        )

    def forward(self, x):
        return self.net(x)


model = BetterApproximation()
criterion = nn.MSELoss()
# Try a slightly smaller learning rate to improve stability over a larger number of epochs
optimizer = optim.Adam(model.parameters(), lr=0.005)

# 3. Training (increase the number of epochs to 3,000)
epochs = 3000
for epoch in range(epochs):
    inputs = torch.FloatTensor(x_scaled)
    targets = torch.FloatTensor(y_scaled)

    optimizer.zero_grad()
    outputs = model(inputs)
    loss = criterion(outputs, targets)
    loss.backward()
    optimizer.step()

    if (epoch + 1) % 500 == 0:
        print(f'Epoch [{epoch + 1}/{epochs}], Loss (MSE): {loss.item():.6f}')

# 4. Reverse scaling for the graph
model.eval()
with torch.no_grad():
    predicted_scaled = model(torch.FloatTensor(x_scaled)).numpy()
    # Reset the data to its original scale
    predicted = scaler_y.inverse_transform(predicted_scaled)

plt.figure(figsize=(10, 5))
plt.scatter(x_raw, y_raw, label='Real Data', s=5, alpha=0.3)
plt.plot(x_raw, predicted, color='red', label='NN Prediction', linewidth=2)
plt.title("Improved Approximation")
plt.legend()
plt.show()