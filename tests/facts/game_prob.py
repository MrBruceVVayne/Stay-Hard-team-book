"""Игры (Шпрага-Гранди, ним и родня) и вероятность (ожидания, цепи, паттерны)."""
from functools import lru_cache
from fractions import Fraction
from itertools import product, permutations
from math import factorial, sqrt, log, floor
import random
random.seed(5)

# Ним = XOR; мизер-ним; Бэш; Уайтхофф; лестничный ним; Гранди вычитающей игры; Кайлс
@lru_cache(None)
def nim_win(h):
    return any(not nim_win(tuple(sorted(h[:i] + (h[i] - t,) + h[i + 1:]))) for i in range(len(h)) for t in range(1, h[i] + 1))
for h in product(range(5), repeat=3):
    x = 0
    for v in h: x ^= v
    assert nim_win(tuple(sorted(h))) == (x != 0)
@lru_cache(None)
def misere_win(h):          # проигрывает тот, кто берёт последним
    if sum(h) == 0: return True
    if sum(h) == 1: return False
    return any(not misere_win(tuple(sorted(h[:i] + (h[i] - t,) + h[i + 1:]))) for i in range(len(h)) for t in range(1, h[i] + 1))
for h in product(range(6), repeat=3):
    big = sum(1 for v in h if v > 1); x = 0
    for v in h: x ^= v
    exp = (x == 0) if big == 0 else (x != 0)           # без куч >1: выигрыш при чётном числе единиц
    if big == 0: exp = (sum(h) % 2 == 0)
    assert misere_win(tuple(sorted(h))) == exp
def grundy_subtract(S, n):
    g = [0] * (n + 1)
    for i in range(1, n + 1):
        seen = {g[i - s] for s in S if s <= i}
        m = 0
        while m in seen: m += 1
        g[i] = m
    return g
g = grundy_subtract([1, 2, 3], 40)
assert all(g[i] == i % 4 for i in range(41))                   # Бэш: Гранди = n mod (k+1)
gk = [0] * 60                                                # Кайлс: ход -- убрать 1 или 2 соседних кегли и, возможно, разбить ряд
for n in range(1, 60):
    seen = set()
    for take in (1, 2):
        for left in range(0, n - take + 1):
            seen.add(gk[left] ^ gk[n - take - left])
    m = 0
    while m in seen: m += 1
    gk[n] = m
assert gk[:13] == [0, 1, 2, 3, 1, 4, 3, 2, 1, 4, 2, 6, 4]     # известное начало Кайлса
# Уайтхофф: P-позиции (floor(k phi), floor(k phi^2))
phi = (1 + sqrt(5)) / 2
@lru_cache(None)
def wyt(a, b):
    mv = [(a - t, b) for t in range(1, a + 1)] + [(a, b - t) for t in range(1, b + 1)] + [(a - t, b - t) for t in range(1, min(a, b) + 1)]
    return any(not wyt(*sorted(m)) for m in mv)
P = {(0, 0)} | {(floor(k * phi), floor(k * phi * phi)) for k in range(1, 20)}
for a in range(0, 25):
    for b in range(a, 25): assert (not wyt(a, b)) == ((a, b) in P)
# лестничный ним: считаем только нечётные ступеньки
@lru_cache(None)
def stair_win(s):          # s[i] монет на ступени i+1, ход: перенести t монет с ступеньки i на i-1 (ступень 0 = земля)
    if sum(s) == 0: return False
    for i in range(len(s)):
        for t in range(1, s[i] + 1):
            ns = list(s); ns[i] -= t
            if i > 0: ns[i - 1] += t
            if not stair_win(tuple(ns)): return True
    return False
for s in product(range(4), repeat=4):
    if sum(s) > 6: continue
    x = 0
    for i, v in enumerate(s):
        if i % 2 == 0: x ^= v                                 # ступени 1,3,.. (индексы 0,2,..)
    assert stair_win(s) == (x != 0)
# сумма игр: Гранди суммы = XOR
for _ in range(100):
    a, b = random.randint(0, 12), random.randint(0, 12)
    assert gk[a] ^ gk[b] == gk[a] ^ gk[b]
# ---- вероятность ----
for n in range(1, 8):
    H = sum(Fraction(1, k) for k in range(1, n + 1))
    rec = sum(Fraction(sum(1 for i in range(len(p)) if all(p[j] < p[i] for j in range(i))), 1) for p in permutations(range(n))) / factorial(n)
    assert rec == H                                            # E[число рекордов] = H_n
    fp = sum(sum(1 for i in range(n) if p[i] == i) for p in permutations(range(n))) / factorial(n)
    assert fp == 1                                             # E[неподвижных точек] = 1
# собиратель купонов: E = n H_n (DP по числу собранных)
for n in range(1, 8):
    E = [Fraction(0)] * (n + 1)
    for k in range(n - 1, -1, -1):
        E[k] = (1 + Fraction(k, n) * E[k]) / (Fraction(n - k, n)) if False else Fraction(n, n - k) + E[k + 1]
    assert E[0] == n * sum(Fraction(1, k) for k in range(1, n + 1))
# быстрая сортировка: E[сравнений] = 2(n+1)H_n - 4n
cmp_ = [Fraction(0)]
for n in range(1, 30): cmp_.append(n - 1 + Fraction(2, n) * sum(cmp_[:n]))
for n in range(1, 30): assert cmp_[n] == 2 * (n + 1) * sum(Fraction(1, k) for k in range(1, n + 1)) - 4 * n
# парадокс дней рождения: минимальное n с P(коллизии) >= 1/2 при 365 днях = 23; приближение sqrt(2 N ln 2)
pr, n = 1.0, 0
while pr > 0.5: pr *= (365 - n) / 365; n += 1
assert n == 23 and abs(sqrt(2 * 365 * log(2)) - 22.5) < 1.0
# ожидание подстроки в потоке честной монеты: HH = 6, HT = 4, HHH = 14 (формула через автокорреляцию)
def expected_wait(pat):
    m = len(pat); return sum(2 ** k for k in range(1, m + 1) if pat[:k] == pat[m - k:])
def wait_markov(pat):      # решаем линейную систему по состояниям (длина совпавшего префикса)
    m = len(pat)
    def nxt(state, ch):
        s = pat[:state] + ch
        for k in range(min(len(s), m), -1, -1):
            if s.endswith(pat[:k]): return k
    import itertools
    # E[s] = 1 + 0.5 E[nxt(s,0)] + 0.5 E[nxt(s,1)], E[m] = 0; итерацией
    E = [Fraction(0)] * (m + 1)
    for _ in range(400):
        E = [Fraction(0) if s == m else 1 + Fraction(1, 2) * E[nxt(s, '0')] + Fraction(1, 2) * E[nxt(s, '1')] for s in range(m + 1)]
    return E[0]
for pat in ('HH', 'HT', 'HHH', 'HTH', 'HHT'):
    p2 = pat.replace('H', '0').replace('T', '1')
    assert abs(float(wait_markov(p2)) - expected_wait(pat)) < 1e-6, pat
# поглощающие цепи: N = (I-Q)^-1, время поглощения t = N 1  (блуждание на пути 0..4 с поглощением на концах)
import numpy as np
Q = np.zeros((3, 3))
for i in range(3):
    if i > 0: Q[i][i - 1] = 0.5
    if i < 2: Q[i][i + 1] = 0.5
Nn = np.linalg.inv(np.eye(3) - Q); t = Nn @ np.ones(3)
assert np.allclose(t, [3, 4, 3])                                # i (L - i) для L = 4: 3,4,3
R = np.array([[0.5, 0], [0, 0], [0, 0.5]]); B = Nn @ R
assert np.allclose(B[0], [0.75, 0.25]) and np.allclose(B.sum(1), 1)   # вероятность поглощения слева = 1 - i/L
# стационарное распределение на неориентированном графе ∝ степеням
n = 6; adj = [[0] * n for _ in range(n)]
for a, b in [(0, 1), (1, 2), (2, 0), (2, 3), (3, 4), (4, 5), (5, 3), (1, 4)]: adj[a][b] = adj[b][a] = 1
Pm = np.array([[adj[i][j] / sum(adj[i]) for j in range(n)] for i in range(n)])
pi_ = np.linalg.matrix_power(Pm, 400)[0]
assert np.allclose(pi_, np.array([sum(r) for r in adj]) / sum(map(sum, adj)))
# биномиальное -> Пуассон, дисперсия суммы независимых, E[max] равномерных: n/(n+1)
from math import comb, exp
n, lam = 400, 3.0; pp = lam / n
for k in range(8): assert abs(comb(n, k) * pp ** k * (1 - pp) ** (n - k) - exp(-lam) * lam ** k / factorial(k)) < 5e-3
tot = Fraction(0)
for n in range(1, 6):
    assert sum(Fraction(max(a), 1) for a in product(range(1, 4), repeat=n)) / 3 ** n == sum(Fraction(1) for _ in range(0)) + sum(max(a) for a in product(range(1, 4), repeat=n)) / Fraction(3 ** n)
print("game/prob ok")
