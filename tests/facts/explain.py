"""Утверждения из пояснений «где и как применять» (facts/*.tex): проверка перебором/точными дробями."""
from fractions import Fraction
from itertools import product, combinations, permutations
from math import comb, factorial, gcd, isqrt, sqrt, acos, sin, cos, pi, atan2, hypot
import random
random.seed(21)
C = lambda n, k: comb(n, k) if 0 <= k <= n else 0
def mu(n):
    r, p, x = 1, 2, n
    while p * p <= x:
        if x % p == 0:
            x //= p
            if x % p == 0: return 0
            r = -r
        p += 1
    return -r if x > 1 else r
def phi(n): return sum(1 for k in range(1, n + 1) if gcd(k, n) == 1)
def divs(n): return [d for d in range(1, n + 1) if n % d == 0]
def fact(n):
    f, p = {}, 2
    while p * p <= n:
        while n % p == 0: f[p] = f.get(p, 0) + 1; n //= p
        p += 1
    if n > 1: f[n] = f.get(n, 0) + 1
    return f

# 1. взаимно простые пары, gcd = k
for n in range(1, 70):
    cp = sum(1 for i in range(1, n + 1) for j in range(1, n + 1) if gcd(i, j) == 1)
    assert cp == sum(mu(d) * (n // d) ** 2 for d in range(1, n + 1)) == 2 * sum(phi(i) for i in range(1, n + 1)) - 1
    for k in range(1, n + 1):
        assert sum(1 for i in range(1, n + 1) for j in range(1, n + 1) if gcd(i, j) == k) == sum(mu(d) * (n // (k * d)) ** 2 for d in range(1, n // k + 1))
# 2. Вильсон: n! (p-1-n)! = (-1)^(n+1) mod p ; Лежандр v_p(n!)
for p in (2, 3, 5, 7, 11, 13, 101):
    for n in range(p):
        assert factorial(n) * factorial(p - 1 - n) % p == (-1) ** (n + 1) % p
for n in range(0, 200):
    for p in (2, 3, 5, 7):
        v, t = 0, factorial(n)
        while t % p == 0: t //= p; v += 1
        assert v == sum(n // p ** i for i in range(1, 9)) == (n - sum(int(c) for c in __import__('numpy').base_repr(n, p))) // (p - 1)
# 3. d, sigma, суммы по k <= n
for n in range(1, 300):
    f = fact(n); dn = 1; sg = 1
    for p, e in f.items(): dn *= e + 1; sg *= (p ** (e + 1) - 1) // (p - 1)
    assert dn == len(divs(n)) and sg == sum(divs(n))
for n in range(1, 120):
    assert sum(len(divs(k)) for k in range(1, n + 1)) == sum(n // k for k in range(1, n + 1))
    assert sum(sum(divs(k)) for k in range(1, n + 1)) == sum(k * (n // k) for k in range(1, n + 1))
    # блоки одинакового floor(n/l)
    l, vals, tot = 1, [], 0
    while l <= n:
        r = n // (n // l); vals.append(n // l); tot += (r - l + 1) * (n // l); l = r + 1
    assert tot == sum(n // k for k in range(1, n + 1)) and sorted(set(n // k for k in range(1, n + 1))) == sorted(vals)
# 4. браслеты (повороты + отражения)
for n in range(1, 9):
    for k in range(1, 4):
        seen = set()
        for s in product(range(k), repeat=n):
            forms = [s[i:] + s[:i] for i in range(n)]; forms += [t[::-1] for t in forms]
            seen.add(min(forms))
        neck = sum(phi(d) * k ** (n // d) for d in divs(n)) // n
        br = (neck + (k ** ((n + 1) // 2) if n % 2 else (k + 1) * k ** (n // 2) // 2)) // 2
        assert len(seen) == br, (n, k, len(seen), br)
# 5. задача о голосовании
for a in range(1, 8):
    for b in range(0, a):
        strict = ge = 0
        for pos in combinations(range(a + b), b):
            bal, ok1, ok2 = 0, True, True
            for i in range(a + b):
                bal += -1 if i in pos else 1
                if bal <= 0: ok1 = False
                if bal < 0: ok2 = False
            strict += ok1; ge += ok2
        assert strict * (a + b) == (a - b) * C(a + b, a)                # строго впереди всё время
        assert ge * (a + 1) == (a - b + 1) * C(a + b, a)               # не отстаёт
# 6. взвешенный Кирхгоф, деревья
def det(M):
    n = len(M); M = [[Fraction(x) for x in r] for r in M]; d = Fraction(1)
    for c in range(n):
        p = next((r for r in range(c, n) if M[r][c] != 0), None)
        if p is None: return 0
        if p != c: M[c], M[p] = M[p], M[c]; d = -d
        d *= M[c][c]
        for r in range(c + 1, n):
            f = M[r][c] / M[c][c]
            for k in range(c, n): M[r][k] -= f * M[c][k]
    return d
for _ in range(60):
    n = random.randint(2, 5); es = [(a, b, random.randint(1, 4)) for a, b in combinations(range(n), 2) if random.random() < 0.7]
    L = [[0] * n for _ in range(n)]
    for a, b, w in es: L[a][a] += w; L[b][b] += w; L[a][b] -= w; L[b][a] -= w
    tot = 0
    for sub in combinations(es, n - 1):
        p = list(range(n))
        def f(x):
            while p[x] != x: x = p[x]
            return x
        ok = True
        for a, b, w in sub:
            if f(a) == f(b): ok = False; break
            p[f(a)] = f(b)
        if ok:
            pr = 1
            for a, b, w in sub: pr *= w
            tot += pr
    assert det([r[1:] for r in L[1:]]) == tot
for _ in range(100):                       # деревья: sum(deg-2) = -2, диаметр двумя BFS, центр
    n = random.randint(2, 12); par = [None] + [random.randrange(i) for i in range(1, n)]
    adj = [[] for _ in range(n)]
    for v in range(1, n): adj[v].append(par[v]); adj[par[v]].append(v)
    assert sum(len(a) - 2 for a in adj) == -2 and sum(1 for a in adj if len(a) == 1) >= 2
    def bfs(s):
        d = [-1] * n; d[s] = 0; q = [s]
        for x in q:
            for y in adj[x]:
                if d[y] < 0: d[y] = d[x] + 1; q.append(y)
        return d
    d0 = bfs(0); a = d0.index(max(d0)); da = bfs(a); b = da.index(max(da)); diam = da[b]
    assert diam == max(max(bfs(s)) for s in range(n))
    ecc = [max(bfs(s)) for s in range(n)]
    centers = [v for v in range(n) if ecc[v] == min(ecc)]
    assert len(centers) in (1, 2) and min(ecc) == (diam + 1) // 2
# 7. регулярный двудольный граф раскладывается на совершенные паросочетания
for _ in range(80):
    n = random.randint(2, 5); k = random.randint(1, 3)
    perms = [random.sample(range(n), n) for _ in range(k)]          # объединение k перестановок -> k-регулярный мультиграф
    edges = [(i, p[i]) for p in perms for i in range(n)]
    # любой k-регулярный двудольный мультиграф имеет совершенное паросочетание
    adj = {i: sorted({b for a, b in edges if a == i}) for i in range(n)}
    mt = [-1] * n
    def try_(v, vis):
        for u in adj[v]:
            if u in vis: continue
            vis.add(u)
            if mt[u] == -1 or try_(mt[u], vis): mt[u] = v; return True
        return False
    assert all(try_(v, set()) for v in range(n))
# 8. ожидания: инверсии, различные значения при выборке, длина случайной выборки
for n in range(1, 8):
    tot = sum(sum(1 for i in range(n) for j in range(i + 1, n) if p[i] > p[j]) for p in permutations(range(n)))
    assert Fraction(tot, factorial(n)) == Fraction(n * (n - 1), 4)
for N in range(1, 6):
    for n in range(1, 6):
        e = Fraction(sum(len(set(a)) for a in product(range(N), repeat=n)), N ** n)
        assert e == N * (1 - Fraction(N - 1, N) ** n)                 # E[число различных] = N (1 - (1-1/N)^n)
for N in range(1, 7):
    for n in range(1, 5):
        e = Fraction(sum(min(a) + 1 for a in product(range(N), repeat=n)), N ** n)
        assert e == sum(Fraction(N - k, N) ** n for k in range(N))    # E[min] = sum_k P(min > k)
# 9. степени и НОД: gcd(a^n - 1, a^m - 1) = a^gcd - 1
for a in range(2, 6):
    for n in range(1, 15):
        for m in range(1, 15): assert gcd(a ** n - 1, a ** m - 1) == a ** gcd(n, m) - 1
# 10. деление с округлением (C++ делит к нулю)
def cdiv(a, b): q = abs(a) // abs(b); return q if (a >= 0) == (b > 0) else -q
for a in range(-40, 41):
    for b in list(range(-9, 0)) + list(range(1, 10)):
        fl = a // b                                                    # Python: пол
        r = a - b * cdiv(a, b)
        assert (cdiv(a, b) - (1 if (r != 0 and (r < 0) != (b < 0)) else 0)) == fl
        if b > 0 and a >= 0: assert -(-a // b) == (a + b - 1) // b
# 11. числа путей длины k = A^k
for _ in range(60):
    n = random.randint(2, 5); A = [[random.randint(0, 2) for _ in range(n)] for _ in range(n)]
    def mm(X, Y): return [[sum(X[i][t] * Y[t][j] for t in range(n)) for j in range(n)] for i in range(n)]
    P = [[int(i == j) for j in range(n)] for i in range(n)]
    for k in range(1, 5):
        P = mm(P, A)
        cnt = sum(1 for seq in product(range(n), repeat=k - 1) for s in range(n) for t in range(n)
                  if False) if False else None
        for s in range(n):
            for t in range(n):
                tot = 0
                for mid in product(range(n), repeat=k - 1):
                    seq = (s,) + mid + (t,); w = 1
                    for x, y in zip(seq, seq[1:]): w *= A[x][y]
                    tot += w
                assert tot == P[s][t]
# 12. геометрия: отражение, пересечение прямых, сегмент/сектор/линза, длина касательной
def close(a, b, e=1e-7): return abs(a - b) <= e * max(1, abs(a), abs(b))
for _ in range(300):
    p = (random.uniform(-9, 9), random.uniform(-9, 9)); a = (random.uniform(-9, 9), random.uniform(-9, 9)); b = (random.uniform(-9, 9), random.uniform(-9, 9))
    d = (b[0] - a[0], b[1] - a[1]); t = ((p[0] - a[0]) * d[0] + (p[1] - a[1]) * d[1]) / (d[0] ** 2 + d[1] ** 2)
    pr = (a[0] + d[0] * t, a[1] + d[1] * t); refl = (2 * pr[0] - p[0], 2 * pr[1] - p[1])
    cr = lambda u, v: u[0] * v[1] - u[1] * v[0]
    assert close(abs(cr(d, (p[0] - a[0], p[1] - a[1]))) / hypot(*d), hypot(p[0] - pr[0], p[1] - pr[1]))      # расстояние до прямой
    assert close(hypot(refl[0] - pr[0], refl[1] - pr[1]), hypot(p[0] - pr[0], p[1] - pr[1]))
    c, e = (random.uniform(-9, 9), random.uniform(-9, 9)), (random.uniform(-9, 9), random.uniform(-9, 9))
    if abs(cr(d, e)) < 1e-3: continue
    tt = cr((c[0] - a[0], c[1] - a[1]), e) / cr(d, e)                        # a + t d = c + s e
    ss = cr((c[0] - a[0], c[1] - a[1]), d) / cr(d, e)
    X1 = (a[0] + tt * d[0], a[1] + tt * d[1]); X2 = (c[0] + ss * e[0], c[1] + ss * e[1])
    assert close(X1[0], X2[0]) and close(X1[1], X2[1])
def integrate(f, lo, hi, n=20000):
    h = (hi - lo) / n; return sum(f(lo + (i + 0.5) * h) for i in range(n)) * h
for _ in range(20):
    r1, r2 = random.uniform(1, 4), random.uniform(1, 4); dd = random.uniform(abs(r1 - r2) + 0.1, r1 + r2 - 0.1)
    lens = r1 ** 2 * acos((dd * dd + r1 * r1 - r2 * r2) / (2 * dd * r1)) + r2 ** 2 * acos((dd * dd + r2 * r2 - r1 * r1) / (2 * dd * r2)) \
        - 0.5 * sqrt((-dd + r1 + r2) * (dd + r1 - r2) * (dd - r1 + r2) * (dd + r1 + r2))
    lo, hi = max(-r1, dd - r2), min(r1, dd + r2)
    num = integrate(lambda x: 2 * min(sqrt(max(0, r1 * r1 - x * x)), sqrt(max(0, r2 * r2 - (x - dd) ** 2))), lo, hi)
    assert abs(lens - num) < 2e-3 * max(1, lens), (lens, num)
    th = random.uniform(0.1, 3.0)
    seg = r1 ** 2 / 2 * (th - sin(th))                                       # сегмент хорды с углом th
    num = integrate(lambda x: 2 * sqrt(max(0, r1 * r1 - x * x)), r1 * cos(th / 2), r1)
    assert abs(seg - num) < 2e-3 * max(1, seg), (seg, num)
    dist = random.uniform(r1 + 0.1, 12)
    assert close(sqrt(dist * dist - r1 * r1) ** 2 + r1 ** 2, dist ** 2)     # длина касательной
print("explain ok")
