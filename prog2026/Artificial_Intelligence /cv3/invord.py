def invord(Oldpop, rate):
    import numpy as np

    Oldpop = np.asarray(Oldpop)
    if Oldpop.ndim == 1:
        Oldpop = Oldpop[None, :]

    lpop, lstring = Oldpop.shape
    rate = 1.0 if rate > 1 else (0.0 if rate < 0 else float(rate))

    n = int(np.ceil(lpop * rate * np.random.rand()))
    Newpop = Oldpop.copy()

    for _ in range(n):
        r  = int(np.ceil(np.random.rand() * lpop)) - 1
        p1 = int(np.ceil(0.001 + np.random.rand() * (lstring - 1)))
        p2 = int(np.ceil(0.001 + np.random.rand() * (lstring - p1))) + p1
        if p1 == lstring: p1 = lstring - 1
        if p2 > lstring:  p2 = lstring

        Newpop[r, p1-1:p2] = Oldpop[r, p1-1:p2][::-1]
        Oldpop = Newpop

    return Newpop