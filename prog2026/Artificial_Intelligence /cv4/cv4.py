from matplotlib import pyplot as plt
import matplotlib.gridspec as gridspec
from numpy.typing import NDArray
import numpy as np

from genetic_all import (
    uniform_space, genrpop,
    seltourn, crossov, muta
)

def compute_violations(chromosome):
    x = chromosome
    s = float(np.sum(x))

    v1 = max(0.0, s - 10_000_000)
    v2 = max(0.0, x[0] + x[1] - 2_500_000)
    v3 = max(0.0, x[4] - x[3])
    v4 = max(0.0, (x[2] + x[3]) - 0.5 * s)

    return [v1, v2, v3, v4]

def check_constraints(chromosome):
    return all(v == 0 for v in compute_violations(chromosome))

def J(x):
    return float(0.04*x[0] + 0.07*x[1] + 0.11*x[2] + 0.06*x[3] + 0.05*x[4])

def fitness_dead(population: NDArray):
    result = []
    for chrom in population:
        viols = compute_violations(chrom)
        if any(v > 0 for v in viols):
            result.append(10_000_001)
        else: result.append(-J(chrom))
    return result

def fitness_step(population: NDArray):
    result = []
    for chrom in population:
        viols = compute_violations(chrom)
        num_violated = sum(1 for v in viols if v > 0)
        if num_violated > 0:
            penalty = 1_000_000 * num_violated
            result.append(penalty)
        else: result.append(-J(chrom))
    return result

def fitness_proportional(population: NDArray):
    result = []
    for chrom in population:
        viols = compute_violations(chrom)
        penalty = sum(viols)
        result.append(-J(chrom) + penalty)
    return result


def run_ga(
        fitness_fn,
        n_genes         = 5,
        pop_size        = 100,
        n_generations   = 300,
        mutation_rate   = 0.05,
        mutation_amp    = 50_000.0,
        crossover_pts   = 2,
        elite_count     = 5,
        lower           = 0,
        upper           = 10_000_000
):
    space       = uniform_space(n_genes, lower, upper)
    population  = genrpop(pop_size, space)
    amp         = np.full(n_genes, mutation_amp)

    fit_history = []

    for gen in range(n_generations):
        # get fitness
        fitness = fitness_fn(population)

        # Recording the best individual (elitism)
        best_idx = np.argmin(fitness)
        fit_history.append(fitness[best_idx])

        # population without elite
        selected_pop, selected_fit = seltourn(
            population, fitness, pop_size - elite_count
        )


        # Crossing over
        crossed_population = crossov(selected_pop.copy(), pts=crossover_pts, mode=0)

        # Mutation
        mutated_population      = muta(crossed_population, mutation_rate, amp, space)

        # New population = elite + mutated
        elite_idx   = np.argsort(fitness)[:elite_count]
        elites      = population[elite_idx]
        population  = np.vstack([elites, mutated_population])

    fitness = fitness_fn(population)

    best_idx = np.argmin(fitness)
    return fitness[best_idx], fit_history, population[best_idx]


def print_result(method_name, best_chrom, best_fit):
    x = best_chrom
    return_val = J(x)
    viols = compute_violations(x)
    feasible = all(v == 0 for v in viols)
    s = np.sum(x)

    print(f"\n{'='*55}")
    print(f"  {method_name}")
    print(f"{'='*55}")
    print(f"  Best fitness (minimized): {best_fit:.4f}")
    print(f"  Return J(x):              {return_val:.2f} EUR")
    print(f"\n  Allocation:")
    labels = ["x1 (Regular events)",
              "x2 (Featured Events)",
              "x3 (Corporate bonds)",
              "x4 (Government bonds)",
              "x5 (Bank savings)"]
    for i, lbl in enumerate(labels):
        print(f"    {lbl:<24} = {x[i]:>14.2f} EUR")
    # TODO: align ✅/❌ to the right side
    print(f"\n  Constraint check:")
    print(f"    P1 (sum ≤ 10M):         sum = {s:.0f} {'✅' if viols[0]==0 else f'❌ viol={viols[0]:.0f}'}")
    print(f"    P2 (x1+x2 ≤ 2.5M):      {x[0]+x[1]:.0f}  {'✅' if viols[1]==0 else f'❌ viol={viols[1]:.0f}'}")
    print(f"    P3 (x4 ≥ x5):           x4={x[3]:.0f}, x5={x[4]:.0f}  {'✅' if viols[2]==0 else f'❌ viol={viols[2]:.0f}'}")
    print(f"    P4 (x3+x4 ≤ 0.5·sum):   {x[2]+x[3]:.0f} ≤ {0.5*s:.0f}  {'✅' if viols[3]==0 else f'❌ viol={viols[3]:.0f}'}")
    print(f"    P5 (xi ≥ 0):            {'✅' if all(xi >= 0 for xi in x) else '❌'}")
    print(f"\n  FEASIBLE:               {'YES ✅' if feasible else 'NO ❌'}")
    return return_val, feasible

def run_experiment():
    N_RUNS = 10
    OPTIMAL_J = 725_000

    GA_PARAMS = dict(
        n_genes         = 5,
        pop_size        = 100,
        n_generations   = 10,
        mutation_rate   = 0.05,
        mutation_amp    = 500_000.0,
        crossover_pts   = 2,
        elite_count     = 5,
        lower           = 0,
        upper           = 10_000_000
    )
    METHODS = [
        ("Dead penalty",         fitness_dead,         "#e74c3c"),
        ("Step penalty",         fitness_step,         "#2ecc71"),
        ("Proportional penalty", fitness_proportional, "#3498db"),
    ]

    # ── Run all experiments ──────────────────────────────────
    all_results = {}
    for method_name, fitness_fn, color in METHODS:
        print(f"\nRunning \"{method_name}\" ({N_RUNS} runs)...")
        runs = []
        for i in range(N_RUNS):
            fit, history, chrom = run_ga(fitness_fn, **GA_PARAMS)
            runs.append((fit, history, chrom))
            print(f"  Run {i+1}/{N_RUNS}: fitness={fit:.4f}, J={J(chrom):.2f}")
        all_results[method_name] = (runs, color)

    # ── Plotting ─────────────────────────────────────────────
    fig = plt.figure(figsize=(18, 14))
    fig.suptitle("Investment Allocation – A Comparison of Penalty Methods\n"
                 f"(GA: pop={GA_PARAMS['pop_size']}, gen={GA_PARAMS['n_generations']}, "
                 f"elite={GA_PARAMS['elite_count']}, mut={GA_PARAMS['mutation_rate']})",
                 fontsize=13, fontweight='bold', y=0.98)

    gs = gridspec.GridSpec(2, 3, figure=fig, hspace=0.4, wspace=0.35)

    best_per_method = {}

    # --- Top row: individual method plots ---
    for col, (method_name, fitness_fn, color) in enumerate(METHODS):
        ax = fig.add_subplot(gs[0, col])
        runs, clr = all_results[method_name]

        best_fit_global = np.inf
        best_chrom_global = None

        for i, (fit, history, chrom) in enumerate(runs):
            label = f"Run {i+1} ({fit:.1f})"
            ax.plot(history, color=clr, alpha=0.6, linewidth=1.2, label=label)
            if fit < best_fit_global:
                best_fit_global = fit
                best_chrom_global = chrom

        best_per_method[method_name] = (best_fit_global, best_chrom_global,
                                        runs[[r[0] for r in runs].index(best_fit_global)][1],
                                        clr)

        ax.axhline(y=-OPTIMAL_J, color='black', linestyle='--',
                   linewidth=1.5, label=f'Optimal J={OPTIMAL_J/1000:.0f}k')
        ax.set_title(method_name, fontsize=11, fontweight='bold')
        ax.set_xlabel("Generation")
        ax.set_ylabel("Fitness (minimized)")
        ax.legend(fontsize=6.5, loc='upper right')
        ax.grid(True, alpha=0.3)
        ax.set_xlim(0, GA_PARAMS['n_generations'])

    # --- Bottom row left+center: comparison plot (spans 2 columns) ---
    ax_cmp = fig.add_subplot(gs[1, :2])
    for method_name, (fit, chrom, history, clr) in best_per_method.items():
        ax_cmp.plot(history, color=clr, linewidth=2.2,
                    label=f"{method_name}  (best fit={fit:.1f}, J={J(chrom):.0f})")

    ax_cmp.axhline(y=-OPTIMAL_J, color='black', linestyle='--',
                   linewidth=1.8, label=f'Analytical optimum J={OPTIMAL_J/1000:.0f}k EUR')
    ax_cmp.set_title("Comparison of best runs – all three penalty methods",
                     fontsize=11, fontweight='bold')
    ax_cmp.set_xlabel("Generation")
    ax_cmp.set_ylabel("Fitness (minimized)")
    ax_cmp.legend(fontsize=9)
    ax_cmp.grid(True, alpha=0.3)
    ax_cmp.set_xlim(0, GA_PARAMS['n_generations'])

    # --- Bottom row right: bar chart of best J per method ---
    ax_bar = fig.add_subplot(gs[1, 2])
    names  = [m[0] for m in METHODS]
    colors = [m[2] for m in METHODS]
    j_vals = [J(best_per_method[n][1]) for n in names]
    feasible_flags = [check_constraints(best_per_method[n][1]) for n in names]

    bars = ax_bar.bar(range(len(names)), j_vals, color=colors, alpha=0.8, edgecolor='black')
    ax_bar.axhline(y=OPTIMAL_J, color='black', linestyle='--',
                   linewidth=1.5, label=f'Optimum {OPTIMAL_J/1000:.0f}k')
    ax_bar.set_xticks(range(len(names)))
    ax_bar.set_xticklabels([n.replace(' ', '\n') for n in names], fontsize=8)
    ax_bar.set_ylabel("Annual return J(x) [EUR]")
    ax_bar.set_title("Best achieved return\nby penalty method", fontsize=10, fontweight='bold')
    ax_bar.legend(fontsize=8)
    ax_bar.grid(True, axis='y', alpha=0.3)

    for i, (bar, j, feas) in enumerate(zip(bars, j_vals, feasible_flags)):
        mark = "✅" if feas else "❌"
        ax_bar.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 2000,
                    f"{j:.0f}\n{mark}", ha='center', va='bottom', fontsize=8, fontweight='bold')

    # ── Print best results per method ────────────────────────
    print("\n" + "="*55)
    print("  SUMMARY OF BEST RESULTS")
    print("="*55)
    for method_name, fitness_fn, _ in METHODS:
        best_fit, best_chrom, _, _ = best_per_method[method_name]
        print_result(method_name, best_chrom, best_fit)

    plt.show()

if __name__ == '__main__':
    run_experiment()
