import numpy as np
import matplotlib.pyplot as plt
from genetic_all import (
    testfn3b, genrpop, uniform_space,
    seltourn, crossov, muta
)

# Jeden beh genetického algoritmu
def run_ga(
    n_genes        = 10,      # počet premenných (dimenzia úlohy)
    pop_size       = 100,     # veľkosť populácie
    n_generations  = 300,     # počet generácií
    crossover_pts  = 2,       # počet bodov kríženia pre crossov()
    mutation_rate  = 0.05,    # pravdepodobnosť mutácie (0–1)
    mutation_amp   = 50.0,    # amplitúda adatívnej mutácie (rozsah +-amp na gén)
    elite_count    = 5,       # počet elitných jedincov (prenesú sa bez zmeny)
    lower          = -1000.0, # dolná hranica priestoru (globálne min. je pri x=−864.7)
    upper          =  1000.0, # horná hranica priestoru
):
    """
    Spustí jeden beh GA a vráti:
        best_x       – gény najlepšieho nájdeného jedinca
        best_fit     – jeho fitness hodnota
        fit_history  – zoznam najlepšej fitness v každej generácii
    """

    # Definícia priestoru a inicializácia populácie
    space = uniform_space(n_genes, lower, upper)           # hranice [2 × n_genes]
    amp   = np.full(n_genes, mutation_amp)                  # amplitúdy mutácie

    population = genrpop(pop_size, space)                  # náhodná počiatočná populácia

    fit_history = []

    # Hlavná slučka GA
    for gen in range(n_generations):
        # Vyhodnotenie fitness
        fitness = testfn3b(population)

        
        # Zaznamenie najlepšieho jedinca (elitizmus)
        best_idx     = np.argmin(fitness)
        best_fit_gen = fitness[best_idx]
        fit_history.append(best_fit_gen)


        # Selekcia – turnajový výber
        #   Vyberieme (pop_size - elite_count) jedincov na reprodukciu.
        #   Elitní jedinci sa prenesú priamo do novej populácie bez zmeny.
        selected_pop, selected_fit = seltourn(
            population, fitness, pop_size - elite_count
        )

        # Kríženie – viacbodové (crossov)
        #   mode=0 -> náhodné párenie jedincov
        offspring = crossov(selected_pop.copy(), pts=crossover_pts, mode=0)

        # Mutácia – adatívna (muta)
        offspring = muta(offspring, rate=mutation_rate, amp=amp, space=space)

        # Nová populácia = elity + potomstvo
        elite_indices = np.argsort(fitness)[:elite_count]
        elites        = population[elite_indices]
        population    = np.vstack([elites, offspring])


    fitness      = testfn3b(population)
    best_idx     = np.argmin(fitness)
    best_x       = population[best_idx]
    best_fit     = fitness[best_idx]

    return best_x, best_fit, fit_history


def run_experiment():

    N_RUNS       = 8          # počet nezávislých spustení GA
    N_GENES      = 10         # dimenzia (10 premenných)
    THEORETICAL  = -792.72 * N_GENES   # ≈ −7927.2
    TARGET       = -7920.0    # postačujúci výsledok podľa zadania

    # Parametre GA (základná konfigurácia)
    GA_PARAMS = dict(
        n_genes       = N_GENES,
        pop_size      = 300,      # väčšia populácia = lepšia diverzita
        n_generations = 500,
        crossover_pts = 2,
        mutation_rate = 0.32,
        mutation_amp  = 100.0,    # väčšia amplitúda pre priestor [-1000, 1000]
        elite_count   = 5,
        lower         = -1000.0,  # priestor musí obsahovať globálne minimum (~-864.7)
        upper         =  1000.0,
    )

    # Spustenie viacerých behov
    all_histories  = []
    all_best_fits  = []
    all_best_xs    = []

    print("=" * 60)
    print(f"Genetický algoritmus – testfn3b  ({N_GENES}D Schwefel)")
    print(f"Teoretické minimum:  {THEORETICAL:.2f}")
    print(f"Cieľová hodnota:     {TARGET:.2f}")
    print("=" * 60)

    for run in range(N_RUNS):
        best_x, best_fit, history = run_ga(**GA_PARAMS)
        all_histories.append(history)
        all_best_fits.append(best_fit)
        all_best_xs.append(best_x)

        reached = "✅ DOSIAHNUTÝ" if best_fit <= TARGET else "❌ nedosiahnutý"
        print(f"Beh {run + 1:2d}:  fitness = {best_fit:10.4f}   cieľ {reached}")

    # Súhrnné výsledky
    overall_best_idx = int(np.argmin(all_best_fits))
    overall_best_fit = all_best_fits[overall_best_idx]
    overall_best_x   = all_best_xs[overall_best_idx]

    print("\n" + "=" * 60)
    print(f"Najlepší výsledok:  fitness = {overall_best_fit:.4f}")
    print(f"Gény chromozómu (X):")
    for i, v in enumerate(overall_best_x):
        print(f"  x{i+1:2d} = {v:10.4f}")
    print("=" * 60)

    # Graf
    fig, ax = plt.subplots(figsize=(12, 6))
    fig.patch.set_facecolor('#0f0f1a')
    ax.set_facecolor('#12122a')

    colors = plt.cm.plasma(np.linspace(0.15, 0.9, N_RUNS))

    for run_idx, history in enumerate(all_histories):
        label = f"Beh {run_idx + 1}  (min={all_best_fits[run_idx]:.1f})"
        # Zvýrazni najlepší beh
        lw    = 2.5 if run_idx == overall_best_idx else 1.0
        alpha = 1.0 if run_idx == overall_best_idx else 0.65
        ax.plot(history, color=colors[run_idx], lw=lw, alpha=alpha, label=label)

    # Referenčné čiary
    ax.axhline(THEORETICAL, color='lime',   lw=1.2, linestyle='--',
               label=f'Teoretické minimum  ({THEORETICAL:.1f})')
    ax.axhline(TARGET,      color='yellow', lw=1.0, linestyle=':',
               label=f'Cieľová hodnota  ({TARGET:.1f})')

    # Styling
    ax.set_title('Genetický algoritmus – testfn3c  (10D Schwefel)\n'
                 'Priebeh fitness v závislosti od počtu generácií',
                 color='#e0e0ff', fontsize=13, pad=12)
    ax.set_xlabel('Generácia', color='#a0a0cc', fontsize=11)
    ax.set_ylabel('Najlepšia fitness', color='#a0a0cc', fontsize=11)
    ax.tick_params(colors='#6060aa', labelsize=9)
    for spine in ax.spines.values():
        spine.set_edgecolor('#2a2a4a')
    ax.grid(True, color='#2a2a4a', linestyle='--', linewidth=0.6)
    ax.legend(fontsize=8, facecolor='#1a1a3a', edgecolor='#3a3a6a',
              labelcolor='#c0c0ee', loc='upper right')

    # Anotácia najlepšieho výsledku
    best_gen = int(np.argmin(all_histories[overall_best_idx]))
    ax.annotate(
        f'  Najlepší: {overall_best_fit:.1f}\n  (beh {overall_best_idx + 1}, gen {best_gen})',
        xy=(best_gen, overall_best_fit),
        xytext=(best_gen + 10, overall_best_fit + 300),
        color='white', fontsize=8,
        arrowprops=dict(arrowstyle='->', color='white', lw=0.8),
    )

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    # run_experiment()
    GA_PARAMS = dict(
        n_genes       = 2,
        pop_size      = 3,      # väčšia populácia = lepšia diverzita
        n_generations = 5,
        crossover_pts = 2,
        mutation_rate = 0.32,
        mutation_amp  = 100.0,    # väčšia amplitúda pre priestor [-1000, 1000]
        elite_count   = 5,
        lower         = -1000.0,  # priestor musí obsahovať globálne minimum (~-864.7)
        upper         =  1000.0,
    )
    run_ga(**GA_PARAMS)
