import numpy as np
import matplotlib.pyplot as plt

def testfn3c(Pop):
    """
    MATLAB/Python:
        - lpop    = počet jedincov (rozmer 1)
        - lstring = počet génov na jedinca (rozmer 2),
                   pričom ak má Pop viac než 2 dimenzie, lstring je
                   súčin rozmerov.

    Fitness výpočet:
        Pre každého jedinca i:
            Fit(i) = sum_{j=1..lstring} ( -(x_ij - x0) * sin( sqrt(abs(x_ij - x0)) ) + y0 )
        kde x0 = 30, y0 = 100.

    Vstup:
        Pop:
            - 2D: tvar (lpop, lstring)
            - 1D: tvar (lstring,) -> interpretuje sa ako 1 jedinec (lpop=1)
            - ND (>=3D): tvar (lpop, d2, d3, ...) -> lstring = d2*d3*...
              a interne sa pre výpočet sploští na (lpop, lstring), aby zodpovedal MATLAB interpretácii.

    Výstup:
        - 1D NumPy pole tvaru (lpop,), t. j. fitness pre každého jedinca.
    """

    Pop = np.asarray(Pop, dtype=float)

    if Pop.ndim == 1:
        # MATLAB: size(1xN) -> lpop=1, lstring=N
        lpop = 1
        lstring = Pop.shape[0]
        X = Pop.reshape(1, lstring)
    else:
        # MATLAB: [m,n]=size(A) pri ND -> n = prod(size(A,2:end))
        lpop = Pop.shape[0]
        lstring = int(np.prod(Pop.shape[1:]))
        X = Pop.reshape(lpop, lstring)

    # --- Výpočet fitness ---
    x0 = 30.0
    y0 = 100.0
    d = X - x0
    Fit = np.sum(-(d) * np.sin(np.sqrt(np.abs(d))) + y0, axis=1)

    return Fit


def hill_algoritm(d_max, dim):
    current_x = np.random.uniform(-1000, 1000, size=dim)
    current_fit = testfn3c(current_x)[0]
    
    history = [current_fit]

    if dim == 1:
        plt.scatter(current_x[0], current_fit, color='black', s=10, zorder=5)
    elif dim == 2:
        plt.scatter(current_x[0], current_x[1], color='black', s=10, zorder=5)

    for _ in range(1000):
        # Random vec
        step = np.random.uniform(-d_max, d_max, size=dim)
        
        next_x = np.clip(current_x + step, -1000, 1000)
        next_fit = testfn3c(next_x)[0]

        if next_fit < current_fit:
            if dim == 2:
                plt.plot([current_x[0], next_x[0]], [current_x[1], next_x[1]], 'r-', alpha=0.3)

            current_x = next_x
            current_fit = next_fit
            
            if dim == 1:
                plt.scatter(current_x[0], current_fit, color='red', s=5, alpha=0.5)
        
        history.append(current_fit)
    if dim == 1:
        plt.scatter(current_x[0], current_fit, color='blue', s=10, zorder=5)
    elif dim == 2:
        plt.scatter(current_x[0], current_x[1], color='blue', s=10, zorder=5)
        
    return current_x, current_fit, history

def multi_restart_hill(n_restarts, d_max, dim):
    best_overall_x = None
    best_overall_fit = float('inf')
    best_history = None

    for _ in range(n_restarts):
        x, fit, hist = hill_algoritm(d_max, dim)
        if fit < best_overall_fit:
            best_overall_fit = fit
            best_overall_x = x
            best_history = hist
            
    return best_overall_x, best_overall_fit, best_history


def run_experiment():
    # --- 1D ---
    plt.figure(figsize=(10, 4))
    x_plot = np.linspace(-1000, 1000, 1000)
    y_plot = testfn3c(x_plot.reshape(-1, 1))
    plt.plot(x_plot, y_plot)
    
    res_x, res_fit, _ = multi_restart_hill(20, 50, 1)
    plt.scatter(res_x, res_fit, color='yellow', s=50, edgecolors='black', zorder=10)
    plt.title("1D Optimization (Multiple Restarts)")
    plt.grid(True)
    plt.axhline(0, color='black', lw=1)
    plt.show()

    # --- 2D Contour Plot ---
    plt.figure(figsize=(8, 6))
    x_range = np.linspace(-1000, 1000, 100)
    X, Y = np.meshgrid(x_range, x_range)
    Z = testfn3c(np.c_[X.ravel(), Y.ravel()]).reshape(X.shape)
    
    plt.contour(X, Y, Z, 20, cmap='viridis', alpha=0.6)
    res_x2, res_fit2, _ = multi_restart_hill(20, 50, 2)
    plt.scatter(res_x2[0], res_x2[1], color='yellow', s=50, edgecolors='black', zorder=10)
    plt.title("2D Contour Plot (Path and Result)")
    plt.show()

    # --- 3D Convergence Plot ---
    plt.figure(figsize=(8, 4))
    _, res_fit3, history = multi_restart_hill(20, 50, 3)
    plt.plot(history, color='purple')
    plt.title("3D Convergence Plot (Fitness value over iterations)")
    plt.xlabel("Iteration")
    plt.ylabel("Fitness")
    plt.grid(True)
    plt.show()

    print(f"1D Result: X={res_x[0]:.2f}, Fit={res_fit:.2f}")
    print(f"2D Result: X={res_x2[0]:.2f}, Fit={res_fit2:.2f}")
    print(f"3D Result: Fit={res_fit3:.2f}")

if __name__ == "__main__":
    run_experiment()
