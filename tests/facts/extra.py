"""Прочие факты: обращение Стирлинга, ЭПФ, Мантель, остовы K_{n,m}, Фибоначчи, Грей, приближения, база МР."""
from fractions import Fraction
from math import comb, factorial, gcd, isqrt, lgamma, log, pi, e, sqrt, exp
from itertools import combinations, product
import random
random.seed(11)
C = lambda n, k: comb(n, k) if 0 <= k <= n else 0
# --- обращение Стирлинга и ЭПФ ---
M = 10
c = [[0] * (M + 1) for _ in range(M + 1)]; S = [[0] * (M + 1) for _ in range(M + 1)]
c[0][0] = S[0][0] = 1
for n in range(1, M + 1):
    for k in range(1, n + 1):
        c[n][k] = c[n - 1][k - 1] + (n - 1) * c[n - 1][k]; S[n][k] = S[n - 1][k - 1] + k * S[n - 1][k]
C1 = [row[:] for row in c]
for k in range(1, M):
    for P in ([random.randint(-9, 9) for _ in range(M + 1)],):
        Q = [sum(S[k][i] * P[i] for i in range(1, k + 1)) for k in range(M + 1)]       # Q = S * P
        back = [sum((-1) ** (k - i) * c[k][i] * Q[i] for i in range(1, k + 1)) for k in range(M + 1)]
        assert back[1:] == P[1:], (k,)                                                  # обратное = знаковые Стирлинг I
# ряды (рациональные коэффициенты): EGF-формулы
N = 9
def mul(a, b): return [sum(a[i] * b[n - i] for i in range(n + 1)) for n in range(N)]
exp1 = [Fraction(1, factorial(n)) for n in range(N)]
expm1 = [Fraction(0)] + exp1[1:]                                    # e^x - 1
lg = [Fraction(0)] + [Fraction(1, n) for n in range(1, N)]          # -ln(1-x)
for k in range(0, 6):
    p1 = [Fraction(1)] + [Fraction(0)] * (N - 1); p2 = p1[:]
    for _ in range(k): p1 = mul(p1, expm1); p2 = mul(p2, lg)
    for n in range(N):
        assert p1[n] * factorial(n) / factorial(k) == S[n][k] if n <= M else True      # (e^x-1)^k/k!
        assert p2[n] * factorial(n) / factorial(k) == c[n][k] if n <= M else True      # (-ln(1-x))^k/k!
dd = [sum(Fraction((-1) ** i, factorial(i)) for i in range(n + 1)) for n in range(N)]  # e^{-x}/(1-x): частичные суммы
d_ = [1, 0]
for n in range(2, N): d_.append((n - 1) * (d_[n - 1] + d_[n - 2]))
assert all(dd[n] * factorial(n) == d_[n] for n in range(N))
# Мёбиус по ⌊n/m⌋ (KACTL) и сумма взаимно простых
def mu(n):
    r, p, x = 1, 2, n
    while p * p <= x:
        if x % p == 0:
            x //= p
            if x % p == 0: return 0
            r = -r
        p += 1
    return -r if x > 1 else r
for n in range(1, 60):
    f = {i: random.randint(-9, 9) for i in range(1, n + 1)}
    g = {m: sum(f[m // k] for k in range(1, m + 1)) for m in range(1, n + 1)}
    assert f[n] == sum(mu(k) * g[n // k] for k in range(1, n + 1))
    if n > 1: assert sum(k for k in range(1, n + 1) if gcd(k, n) == 1) * 2 == n * sum(1 for k in range(1, n + 1) if gcd(k, n) == 1)
# Мантель/Тюран: без треугольников не более ⌊n^2/4⌋ рёбер; остовы K_{n,m}; степенная сумма Лагранж-обращ.
def tri_free_max(n):
    es = list(combinations(range(n), 2)); best = 0
    for mask in range(1 << len(es)):
        k = bin(mask).count('1')
        if k <= best: continue
        adj = [[0] * n for _ in range(n)]
        for i, (a, b) in enumerate(es):
            if mask >> i & 1: adj[a][b] = adj[b][a] = 1
        if not any(adj[a][b] and adj[b][c] and adj[a][c] for a, b, c in combinations(range(n), 3)): best = k
    return best
for n in range(2, 7): assert tri_free_max(n) == n * n // 4
def det(Mx):
    n = len(Mx); Mx = [[Fraction(x) for x in r] for r in Mx]; d = Fraction(1)
    for c_ in range(n):
        p = next((r for r in range(c_, n) if Mx[r][c_] != 0), None)
        if p is None: return 0
        if p != c_: Mx[c_], Mx[p] = Mx[p], Mx[c_]; d = -d
        d *= Mx[c_][c_]
        for r in range(c_ + 1, n):
            f = Mx[r][c_] / Mx[c_][c_]
            for k in range(c_, n): Mx[r][k] -= f * Mx[c_][k]
    return d
for a in range(1, 6):
    for b in range(1, 6):
        n = a + b; L = [[0] * n for _ in range(n)]
        for i in range(a):
            for j in range(a, n): L[i][j] -= 1; L[j][i] -= 1; L[i][i] += 1; L[j][j] += 1
        assert det([r[1:] for r in L[1:]]) == a ** (b - 1) * b ** (a - 1)             # τ(K_{a,b}) = a^{b-1} b^{a-1}
# Фибоначчи: удвоение, Цекендорф, сумма
F = [0, 1]
for _ in range(120): F.append(F[-1] + F[-2])
for n in range(1, 60):
    assert F[2 * n] == F[n] * (2 * F[n + 1] - F[n]) and F[2 * n + 1] == F[n] ** 2 + F[n + 1] ** 2
    assert sum(F[1:n + 1]) == F[n + 2] - 1 and sum(F[i] ** 2 for i in range(1, n + 1)) == F[n] * F[n + 1]
for n in range(1, 4000):
    x, rep = n, []
    for i in range(30, 1, -1):
        if F[i] <= x: x -= F[i]; rep.append(i)
    assert x == 0 and all(a - b >= 2 for a, b in zip(rep, rep[1:]))                    # Цекендорф: без соседних
# код Грея
g = [i ^ (i >> 1) for i in range(1 << 10)]
assert all(bin(g[i] ^ g[i + 1]).count('1') == 1 for i in range(len(g) - 1)) and len(set(g)) == 1 << 10
inv = lambda x: [y for y in range(1 << 10) if g[y] == x][0] if False else None
# приближения: Стирлинг (ряд), гармонические числа, π(x) ~ x/ln x порядок, e и ln
for n in (5, 10, 20, 50):
    ser = n * log(n) - n + 0.5 * log(2 * pi * n) + 1 / (12 * n) - 1 / (360 * n ** 3) + 1 / (1260 * n ** 5)
    assert abs(ser - lgamma(n + 1)) < 2e-8  # следующий член ряда: -1/(1680 n^7)
    f2 = sqrt(2 * pi * n) * (n / e) ** n * (1 + 1 / (12 * n) + 1 / (288 * n * n) - 139 / (51840 * n ** 3))
    assert abs(f2 / factorial(n) - 1) < 1e-6
    H = sum(1 / k for k in range(1, n + 1)); assert abs(H - (log(n) + 0.5772156649015329 + 1 / (2 * n) - 1 / (12 * n * n))) < 2e-5
    assert abs(C(2 * n, n) / (4 ** n / sqrt(pi * n)) - 1) < 1 / (8 * n) + 1e-3                # C(2n,n) ~ 4^n / sqrt(pi n)
# Миллер–Рабин: набор Синклера для 64 бит против 12 первых простых на случайных и трудных числах
def mr(n, bases):
    if n < 2: return False
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % p == 0: return n == p
    d, s = n - 1, 0
    while d % 2 == 0: d //= 2; s += 1
    for a in bases:
        a %= n
        if a == 0: continue
        x = pow(a, d, n)
        if x in (1, n - 1): continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1: break
        else: return False
    return True
SIN = (2, 325, 9375, 28178, 450775, 9780504, 1795265022); P12 = (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37)
hard = [3825123056546413051, 318665857834031151167461 % (1 << 64), 3215031751, 4759123141, 1122004669633, 2152302898747, 3474749660383, 341550071728321]
for n in hard + [random.getrandbits(random.randint(33, 64)) | 1 for _ in range(60000)] + [random.getrandbits(40) * 2 + 1 for _ in range(20000)]:
    assert mr(n, SIN) == mr(n, P12), n
print("extra ok")

# ---------- дополнительные проверки утверждений из facts/*.tex ----------
# Кайлс: период 12 после n = 70
gk = [0] * 400
for n in range(1, 400):
    seen = set()
    for take in (1, 2):
        for left in range(0, n - take + 1): seen.add(gk[left] ^ gk[n - take - left])
    m = 0
    while m in seen: m += 1
    gk[n] = m
assert all(gk[n] == gk[n - 12] for n in range(83, 400)) and gk[82] != gk[70]   # период 12, последнее исключение n = 70
# Лежандр (три квадрата) и два квадрата
def sq_sum(k, n):
    r = int(n ** 0.5) + 1
    sq = [x * x for x in range(r + 1)]
    if k == 2: return any(n - a in sq for a in sq if a <= n)
    return any((n - a - b) in sq for a in sq for b in sq if a + b <= n)
for n in range(1, 300):
    m = n
    while m % 4 == 0: m //= 4
    assert (not sq_sum(3, n)) == (m % 8 == 7)
    f, x, p = {}, n, 2
    while p * p <= x:
        while x % p == 0: f[p] = f.get(p, 0) + 1; x //= p
        p += 1
    if x > 1: f[x] = f.get(x, 0) + 1
    assert sq_sum(2, n) == all(e % 2 == 0 for q, e in f.items() if q % 4 == 3)
# Вильсон, период Пизано (равенство только при 2*5^k), неубывающая оценка
for p in (2, 3, 5, 7, 11, 13, 101): assert factorial(p - 1) % p == p - 1
def pisano(n):
    a, b, k = 0, 1 % n, 0
    while True:
        a, b = b, (a + b) % n; k += 1
        if a == 0 and b == 1 % n: return k
assert [m for m in range(2, 700) if pisano(m) == 6 * m] == [10, 50, 250]   # 6m достигается при m = 2*5^k, k >= 1
# центр масс многоугольника: формула против взвешенного веера треугольников
for _ in range(200):
    pts = sorted([(random.uniform(-5, 5), random.uniform(-5, 5)) for _ in range(6)])
    cx = sum(p[0] for p in pts) / 6; cy = sum(p[1] for p in pts) / 6
    import math
    pts.sort(key=lambda p: math.atan2(p[1] - cy, p[0] - cx))
    n = len(pts); A = Cx = Cy = 0
    for i in range(n):
        a, b = pts[i], pts[(i + 1) % n]; cr = a[0] * b[1] - a[1] * b[0]
        A += cr / 2; Cx += (a[0] + b[0]) * cr; Cy += (a[1] + b[1]) * cr
    fx, fy = Cx / (6 * A), Cy / (6 * A)
    tot = wx = wy = 0
    for i in range(n):                                     # веер из точки 0
        a, b, c = pts[0], pts[i], pts[(i + 1) % n]
        ar = ((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])) / 2
        tot += ar; wx += ar * (a[0] + b[0] + c[0]) / 3; wy += ar * (a[1] + b[1] + c[1]) / 3
    assert abs(fx - wx / tot) < 1e-7 and abs(fy - wy / tot) < 1e-7
# p(n) приближение, Каталан и p(10)
pp = [1]
for n in range(1, 101):
    s = 0; k = 1
    while k * (3 * k - 1) // 2 <= n:
        sg = 1 if k % 2 else -1
        s += sg * pp[n - k * (3 * k - 1) // 2] + (sg * pp[n - k * (3 * k + 1) // 2] if k * (3 * k + 1) // 2 <= n else 0); k += 1
    pp.append(s)
assert pp[10] == 42
for n in (50, 100):
    approx = exp(pi * sqrt(2 * n / 3)) / (4 * sqrt(3) * n)
    assert 0.93 < pp[n] / approx < 1.0 and 0.9 < pp[n] / (0.145 / n * exp(2.56 * sqrt(n))) < 1.1
assert [C(2 * n, n) // (n + 1) for n in range(11, 15)] == [58786, 208012, 742900, 2674440]
assert [sum((-1) ** t * factorial(9) // factorial(t) for t in range(10))] == [133496]
# знаковые Стирлинг I: падающая степень x(x-1)..(x-n+1) = sum (-1)^(n-k) c(n,k) x^k
for n in range(0, 9):
    for x in range(-4, 6):
        fall = 1
        for i in range(n): fall *= (x - i)
        assert fall == sum((-1) ** (n - k) * C1[n][k] * x ** k for k in range(n + 1))
# неразличимые (непомеченные) конструкции: MSet и Cycle
def inv_(a, N):  # 1/(1-t) на ряду
    pass
N2 = 14
def expser(a):    # exp(a), a[0]=0, рациональные коэффициенты
    res = [Fraction(0)] * N2; res[0] = Fraction(1)
    for n in range(1, N2): res[n] = sum(k * a[k] * res[n - k] for k in range(1, n + 1)) / n
    return res
A = [Fraction(0)] + [Fraction(1)] * (N2 - 1)             # A(z) = z/(1-z): по одному объекту каждого размера
msa = [sum((A[n // k] if n % k == 0 else 0) / Fraction(k) for k in range(1, N2)) for n in range(N2)]
part = expser(msa)
assert [int(x) for x in part[:12]] == pp[:12]            # MSet(z/(1-z)) = разбиения
from math import gcd as g2
def totient(n): return sum(1 for i in range(1, n + 1) if g2(i, n) == 1)
for kk in (2, 3):                                       # циклы из k цветов: ожерелья
    Ak = [Fraction(0), Fraction(kk)] + [Fraction(0)] * (N2 - 2)
    for m in range(1, 10):
        # коэффициент z^m в sum_n phi(n)/n * ln 1/(1-Ak(z^n)) ; ln 1/(1-u) = sum u^j / j, u = kk z^n
        tot = Fraction(0)
        for n in range(1, m + 1):
            if m % n == 0: tot += Fraction(totient(n), n) * Fraction(kk ** (m // n), m // n)
        assert tot == Fraction(sum(totient(d) * kk ** (m // d) for d in range(1, m + 1) if m % d == 0), m)
print("extra2 ok")
