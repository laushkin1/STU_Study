from datetime import datetime

import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

from genetic_all import genrpop_perm, selbest, seltourn
from invord import invord


B=[
    [0,0], [17,100], [51,15], [70,62], [42,25],
    [32,17], [51,64], [39,45], [68,89], [20,19],
    [12,87], [80,37], [35,82], [2,15], [38,95],
    [33,50], [85,52], [97,27], [99,10], [37,67],
    [20,82], [49,0], [62,14], [7,60],
    [0,0]
]

pts = np.array(B, dtype=float)
# Precompute distance matrix (25x25) for fast lookups
D = np.zeros((25, 25))
for _i in range(25):
    for _j in range(25):
        D[_i, _j] = np.linalg.norm(pts[_i] - pts[_j])

def ox_crossover(pop, rate):
    """
    Order Crossover (OX) - permutation-safe crossover.

    Why standard crossov() cannot be used:
      Copying gene values directly produces offspring with duplicate cities
      and missing cities — an invalid permutation. OX keeps a sub-segment
      from one parent and fills the rest from the other parent in original
      relative order, guaranteeing every city appears exactly once.
    """
    pop = pop.copy()
    n, l = pop.shape
    indices = np.random.permutation(n)
    for k in range(0, int(n * rate) - 1, 2):
        i, j = indices[k], indices[k + 1]
        p1, p2 = pop[i].copy(), pop[j].copy()
        a, b = sorted(np.random.choice(l, 2, replace=False))
        for p_src, p_dst, child in [(p1, p2, i), (p2, p1, j)]:
            seg = p_src[a:b + 1]
            fill = [x for x in np.roll(p_dst, -(b + 1)) if x not in seg]
            c = p_src.copy()
            c[a:b + 1] = seg
            positions = [(b + 1 + x) % l for x in range(l - (b - a + 1))]
            for idx, pos in enumerate(positions):
                c[pos] = fill[idx]
            pop[child] = c
    return pop

def route_len(pop: NDArray) -> float:
    full = np.array([0] + list(pop) + [24], dtype=int)
    return float(D[full[:-1], full[1:]].sum())

def test_fitness(populations: NDArray) -> list[float]:
    return [route_len(p) for p in populations]

def test_fitness_old(populations: NDArray) -> list[int]:
    dist_history = []
    # sqrt((x2-x1)**2 + (y2-y1)**2)
    for pop in populations:
        len_pop = len(pop)
        distance = np.sqrt( (B[pop[0]][0]-B[0][0])**2 + (B[pop[0]][1]-B[0][1])**2 )
        for i in range(len_pop - 1):
            p1 = B[pop[i]]
            p2 = B[pop[i+1]]
            x1, y1 = p1[0], p1[1]
            x2, y2 = p2[0], p2[1]
            d = np.sqrt( (x2-x1)**2 + (y2-y1)**2 )
            distance += d
        distance += np.sqrt( (B[24][0]-B[pop[len_pop-1]][0])**2 + (B[24][1]-B[pop[len_pop-1]][1])**2 )
        dist_history.append(distance)

    return dist_history

def run_ga(
        pop_size=100,
        n_generations=300,
        mutation_rate=0.05,
        crossover_rate=0.9,
        elite_count=5
):

    # First populations
    populations = genrpop_perm(pop_size, 1, 23)
    fitness = test_fitness(populations)
    fit_history = []

    for gen in range(n_generations):
        # Elitarismus
        elite_populations, elite_fitness = selbest(populations, fitness, [elite_count])

        # population without elite
        parents_population, parents_fitness = seltourn(
            populations, fitness, len(populations) - len(elite_populations)
        )

        # krizenie
        crossed_population = ox_crossover(parents_population, crossover_rate)

        # mutacia
        mutated_population = invord(crossed_population, mutation_rate)

        # new population = elite+mutated
        elite_arr   = np.array(elite_populations).reshape(elite_count, -1)
        mutated_arr = np.array(mutated_population).reshape(len(mutated_population), -1)
        populations = np.vstack([elite_arr, mutated_arr])
        fitness = test_fitness(populations)

        fit_history.append(min(fitness))

    best_idx = int(np.argmin(fitness))
    return fitness[best_idx], fit_history, populations[best_idx]

#######################################################################

def plot_convergence(all_histories, all_fits, target=480):
    """Plot 1: fitness vs generation for all runs + average curve."""
    fig, ax = plt.subplots(figsize=(10, 6))
    colors = plt.cm.tab10(np.linspace(0, 1, len(all_histories)))

    for i, (hist, fit) in enumerate(zip(all_histories, all_fits)):
        label = f"Run {i+1}  ({fit:.1f})" + ("  ✓" if fit <= target else "")
        ax.plot(hist, color=colors[i], alpha=0.55, linewidth=1.2, label=label)

    avg = np.mean(all_histories, axis=0)
    ax.plot(avg, color='black', linewidth=1.5, linestyle='--',
            label='Average', zorder=10)
    ax.axhline(target, color='red', linewidth=1.5, linestyle=':',
               label=f'Target = {target}')

    ax.set_xlabel('Generation', fontsize=12)
    ax.set_ylabel('Fitness (route length)', fontsize=12)
    ax.set_title('GA Convergence – Fitness vs. Generation (10 runs)', fontsize=13)
    ax.legend(fontsize=8, ncol=2, loc='upper right')
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    return fig

def plot_final_fitness(all_fits, target=480):
    """Plot 2: bar chart of final fitness value per run."""
    fig, ax = plt.subplots(figsize=(9, 5))
    bar_colors = ['#2ecc71' if f <= target else '#e74c3c' for f in all_fits]
    bars = ax.bar(range(1, len(all_fits) + 1), all_fits,
                  color=bar_colors, edgecolor='black', linewidth=0.7)
    ax.axhline(target, color='red', linewidth=1.5, linestyle='--',
               label=f'Target = {target}')

    for bar, val in zip(bars, all_fits):
        ax.text(bar.get_x() + bar.get_width() / 2,
                bar.get_height() + 1.5, f'{val:.1f}',
                ha='center', va='bottom', fontsize=8.5)

    n_ok = sum(1 for f in all_fits if f <= target)
    green_p = mpatches.Patch(color='#2ecc71', label=f'≤ {target}  (reached)')
    red_p   = mpatches.Patch(color='#e74c3c', label=f'> {target}  (not reached)')
    ax.legend(handles=[green_p, red_p], fontsize=10)
    ax.set_xlabel('Run', fontsize=12)
    ax.set_ylabel('Final fitness (route length)', fontsize=12)
    ax.set_title(
        f'Final Fitness per Run  –  {n_ok}/{len(all_fits)} runs reached target',
        fontsize=13)
    ax.set_xticks(range(1, len(all_fits) + 1))
    ax.set_ylim(400, max(all_fits) * 1.1)
    ax.grid(True, axis='y', alpha=0.3)
    fig.tight_layout()
    return fig

def plot_best_route(best_genome, best_fit):
    """Plot 3: visualise the best route found in the 2D plane."""
    fig, ax = plt.subplots(figsize=(9, 9))
    full_path = [0] + list(best_genome) + [24]
    route_pts = pts[full_path]

    ax.plot(route_pts[:, 0], route_pts[:, 1], 'b-',
            linewidth=1.8, alpha=0.65, zorder=1)
    ax.plot(route_pts[:, 0], route_pts[:, 1], 'o',
            color='royalblue', markersize=7, zorder=2)
    ax.plot(0, 0, 's', color='green', markersize=13, zorder=3,
            label='Start / End  [0, 0]')

    # Label every city with its 1-based index from the assignment
    for idx_pt, (x, y) in enumerate(pts):
        ax.annotate(str(idx_pt + 1), (x, y),
                    textcoords='offset points', xytext=(5, 5),
                    fontsize=8, color='darkred', fontweight='bold')

    ax.set_xlabel('x', fontsize=12)
    ax.set_ylabel('y', fontsize=12)
    ax.set_title(f'Best Route  –  Length = {best_fit:.4f}', fontsize=13)
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    return fig

#######################################################################

def run_experiment():
    N_RUNS = 10
    TARGET = 480

    GA_PARAMS = dict(
        pop_size=200,
        n_generations=500,
        mutation_rate=0.25,
        crossover_rate=0.9,
        elite_count=10
    )

    all_histories = []
    all_fits = []
    all_genomes = []

    print("=" * 60)
    print(f"Target value: {TARGET:.2f}")
    print("=" * 60)

    for run in range(N_RUNS):
        fit, fit_history, genome = run_ga(**GA_PARAMS)
        all_histories.append(fit_history)
        all_fits.append(fit)
        all_genomes.append(genome)

        reached = "✅ REACHED" if fit <= TARGET else "❌ not reached"
        print(f"Run {run + 1:2d}:  fitness = {fit:10.4f}   goal {reached}")

    overall_best_idx = int(np.argmin(all_fits))
    overall_best_fit = all_fits[overall_best_idx]
    overall_best_genome = all_genomes[overall_best_idx]
    n_reached = sum(1 for f in all_fits if f <= TARGET)

    print("\n" + "=" * 60)
    print(f"Best result:     fitness = {overall_best_fit:.4f}")
    print(f"Runs <= {TARGET}: {n_reached}/{N_RUNS} ({100 * n_reached / N_RUNS:.0f}%)")
    print(f"Best genome: [0 {" ".join(map(str, overall_best_genome))} 24]")
    print(f"Road: {B[0]} -> {" -> ".join(map(str, [B[i] for i in overall_best_genome]))} -> {B[24]}")
    print("=" * 60)

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    # --- Graphs ---
    fig1 = plot_convergence(all_histories, all_fits, TARGET)
    fig2 = plot_final_fitness(all_fits, TARGET)
    fig3 = plot_best_route(overall_best_genome, overall_best_fit)

    # fig1.savefig(f'pdf/plot_convergence_{timestamp}.png',   dpi=150, bbox_inches='tight')
    # fig2.savefig(f'pdf/plot_bars.png_{timestamp}.png',      dpi=150, bbox_inches='tight')
    # fig3.savefig(f'pdf/plot_route.png_{timestamp}.png',     dpi=150, bbox_inches='tight')
    # print("\nGraphs saved to pdf folder: plot_convergence_Ymd_HMS.png, plot_bars_Ymd_HMS.png, plot_route_Ymd_HMS.png")

    plt.show()


if __name__ == "__main__":
    run_experiment()
