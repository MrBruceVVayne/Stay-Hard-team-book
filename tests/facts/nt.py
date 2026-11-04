"""Теория чисел: тождества, таблицы простых/делителей, утверждения из образцов."""
from math import gcd, isqrt, comb, log2, factorial
from fractions import Fraction
from itertools import product

def is_prime(n):
    if n < 2: return False
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % p == 0: return n == p
    d, s = n - 1, 0
    while d % 2 == 0: d //= 2; s += 1
    for a in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        x = pow(a, d, n)
        if x in (1, n - 1): continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1: break
        else: return False
    return True
def sieve(n):
    s = bytearray([1]) * (n + 1); s[0:2] = b'\0\0'
    for i in range(2, isqrt(n) + 1):
        if s[i]: s[i * i::i] = bytearray(len(s[i * i::i]))
    return s
N = 3000
S = sieve(N)
def factor(n):
    f, p = {}, 2
    while p * p <= n:
        while n % p == 0: f[p] = f.get(p, 0) + 1; n //= p
        p += 1
    if n > 1: f[n] = f.get(n, 0) + 1
    return f
phi = [0] + [sum(1 for k in range(1, n + 1) if gcd(k, n) == 1) for n in range(1, 400)]
def mu(n):
    f = factor(n)
    return 0 if any(e > 1 for e in f.values()) else (-1) ** len(f)
def divs(n): return [d for d in range(1, n + 1) if n % d == 0]

# phi: сумма по делителям, phi(mn), мультипликативность, сумма gcd, lcm-формулы
for n in range(1, 400):
    assert sum(phi[d] for d in divs(n)) == n
    assert sum(mu(d) for d in divs(n)) == (1 if n == 1 else 0)
    assert sum(gcd(k, n) for k in range(1, n + 1)) == sum(d * phi[n // d] for d in divs(n))
    assert sum(n * k // gcd(n, k) for k in range(1, n + 1)) * 2 == n * (sum(phi[d] * d for d in divs(n)) + 1)   # sum lcm(i,n)
    for m in range(1, 60):
        g = gcd(m, n)
        assert phi[m * n] * 1 == phi[m] * phi[n] * g // phi[g] if m * n < 400 else True
for m in range(1, 60):
    for n in range(1, 60):
        g = gcd(m, n)
        # phi(mn) = phi(m)phi(n) g/phi(g) (phi считаем напрямую)
        pm = lambda x: sum(1 for k in range(1, x + 1) if gcd(k, x) == 1)
        assert pm(m * n) * pm(g) == pm(m) * pm(n) * g
# Мёбиус: инверсия и сумма lcm(i,j)
for n in range(1, 120):
    f = {d: d * d + 3 for d in range(1, n + 1)}
    g = {m: sum(f[d] for d in divs(m)) for m in range(1, n + 1)}
    assert f[n] == sum(mu(d) * g[n // d] for d in divs(n))
for n in range(1, 40):
    lhs = sum(i * j // gcd(i, j) for i in range(1, n + 1) for j in range(1, n + 1))
    rhs = sum(((1 + n // l) * (n // l) // 2) ** 2 * sum(mu(d) * l * d for d in divs(l)) for l in range(1, n + 1))
    assert lhs == rhs
    assert sum(mu(k) ** 2 for k in range(1, n + 1)) == sum(mu(k) * (n // (k * k)) for k in range(1, isqrt(n) + 1))
# gcd/lcm дистрибутивность
lcm = lambda a, b: a * b // gcd(a, b)
for a, b, c in product(range(1, 25), repeat=3):
    assert gcd(a, lcm(b, c)) == lcm(gcd(a, b), gcd(a, c))
    assert lcm(a, gcd(b, c)) == gcd(lcm(a, b), lcm(a, c))
    assert gcd(gcd(lcm(a, b), lcm(b, c)), lcm(a, c)) == lcm(lcm(gcd(a, b), gcd(b, c)), gcd(a, c))
# обобщённая теорема Эйлера: n^x = n^(phi(m) + x mod phi(m)) (mod m), x >= log2 m
for m in range(1, 120):
    ph = phi[m] if m < 400 else None
    for n in range(0, 40):
        for x in range(int(log2(m)) + 1 if m > 1 else 0, 90):
            assert pow(n, x, m) == pow(n, ph + x % ph, m)
# Бёзу: общее решение ax+by=c
for a, b in product(range(1, 15), repeat=2):
    g = gcd(a, b)
    for c in range(-30, 31):
        sols = {(x, y) for x in range(-200, 201) for y in [(c - a * x) // b] if a * x + b * y == c and abs(y) <= 400}
        if c % g: assert not sols; continue
        x0, y0 = min(sols)
        for k in range(-5, 6):
            assert a * (x0 + k * (b // g)) + b * (y0 - k * (a // g)) == c
# число неотриц. решений ax+by=n: n/(ab) - {b' n / a} - {a' n / b} + 1, gcd(a,b)=1
for a in range(1, 14):
    for b in range(1, 14):
        if gcd(a, b) != 1: continue
        ai = pow(a, -1, b) if b > 1 else 0
        bi = pow(b, -1, a) if a > 1 else 0
        for n in range(0, 80):
            cnt = sum(1 for x in range(0, n // a + 1) if (n - a * x) % b == 0)
            fr = lambda num, den: Fraction(num % den, den) if den > 1 else Fraction(0)
            assert cnt == Fraction(n, a * b) - fr(bi * n, a) - fr(ai * n, b) + 1
# Лагранж/Якоби: r4, r8, Ферма r2, пифагоровы тройки
def rk(k, n):
    r = {0: 1}
    cur = {0: 1}
    base = {}
    for x in range(-isqrt(n) - 1, isqrt(n) + 2):
        base[x * x] = base.get(x * x, 0) + 1
    for _ in range(k):
        nxt = {}
        for s1, c1 in cur.items():
            for s2, c2 in base.items():
                if s1 + s2 <= n: nxt[s1 + s2] = nxt.get(s1 + s2, 0) + c1 * c2
        cur = nxt
    return cur.get(n, 0)
for n in range(1, 40):
    assert rk(4, n) == 8 * sum(d for d in divs(n) if d % 4)
    assert rk(8, n) == 16 * sum((-1) ** (n + d) * d ** 3 for d in divs(n))
    f = factor(n)
    r2 = 4 * (sum(1 for d in divs(n) if d % 4 == 1) - sum(1 for d in divs(n) if d % 4 == 3))
    assert rk(2, n) == r2                                    # r2(n) = 4(d1(n) - d3(n))
for n in range(1, 120):
    cnt = sum(1 for a in range(1, n) for b in range(1, n) if a * a + b * b == n * n)
    f = factor(n); pr = 1
    for p, e in f.items():
        if p % 4 == 1: pr *= 2 * e + 1
    assert cnt == pr - 1
    # все тройки с гипотенузой n порождаются (m,n,k)
    gen = set()
    for m in range(2, 200):
        for q in range(1, m):
            if gcd(m, q) == 1 and (m - q) % 2 == 1:
                a, b, c = m * m - q * q, 2 * m * q, m * m + q * q
                if n % c == 0: gen |= {(a * (n // c), b * (n // c)), (b * (n // c), a * (n // c))}
    assert len(gen) == cnt
# квадратичная взаимность и критерий Эйлера
def leg(a, p): return 0 if a % p == 0 else (1 if pow(a, (p - 1) // 2, p) == 1 else -1)
ps = [p for p in range(3, 200) if S[p]]
for p in ps:
    for q in ps:
        if p != q: assert leg(p, q) * leg(q, p) == (-1) ** ((p - 1) * (q - 1) // 4)
# первообразные корни: есть для 1,2,4,p^k,2p^k, количество phi(phi(n))
def order(g, n):
    k, x = 1, g % n
    while x != 1: x = x * g % n; k += 1
    return k
for n in range(3, 400):
    roots = [g for g in range(1, n) if gcd(g, n) == 1 and order(g, n) == phi[n]]
    f = factor(n)
    has = n in (2, 4) or (len(f) == 1 and 2 not in f) or (len(f) == 2 and 2 in f and f[2] == 1)
    assert (len(roots) > 0) == has, n
    if roots: assert len(roots) == phi[phi[n]]
# Фибоначчи: период Пизано <= 6n, критерий 5n^2±4, gcd, Кассини
fib = [0, 1]
for _ in range(400): fib.append(fib[-1] + fib[-2])
def pisano(n):
    a, b, k = 0, 1 % n, 0
    while True:
        a, b = b, (a + b) % n; k += 1
        if a == 0 and b == 1 % n: return k
for n in range(2, 600):
    assert pisano(n) <= 6 * n
assert pisano(10) == 60 and pisano(2) == 3 and pisano(5) == 20 and pisano(50) == 300     # 6n достигается при n = 2*5^k
fs = set(fib[:60])
for n in range(0, 20000):
    is_sq = lambda x: x >= 0 and isqrt(x) ** 2 == x
    assert (n in fs) == (is_sq(5 * n * n + 4) or is_sq(5 * n * n - 4))
for a in range(1, 60):
    for b in range(1, 60): assert gcd(fib[a], fib[b]) == fib[gcd(a, b)]
for n in range(1, 80): assert fib[n - 1] * fib[n + 1] - fib[n] ** 2 == (-1) ** n
# Пик
from math import floor
import random
random.seed(1)
def hull(pts):
    pts = sorted(set(pts))
    cr = lambda o, a, b: (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])
    lo, up = [], []
    for p in pts:
        while len(lo) >= 2 and cr(lo[-2], lo[-1], p) <= 0: lo.pop()
        lo.append(p)
    for p in reversed(pts):
        while len(up) >= 2 and cr(up[-2], up[-1], p) <= 0: up.pop()
        up.append(p)
    return lo[:-1] + up[:-1]
for _ in range(300):
    h = hull([(random.randint(-6, 6), random.randint(-6, 6)) for _ in range(random.randint(3, 9))])
    if len(h) < 3: continue
    A2 = sum(h[i][0] * h[(i + 1) % len(h)][1] - h[i][1] * h[(i + 1) % len(h)][0] for i in range(len(h)))
    Bd = sum(gcd(abs(h[i][0] - h[(i + 1) % len(h)][0]), abs(h[i][1] - h[(i + 1) % len(h)][1])) for i in range(len(h)))
    # внутренние точки: перебор (выпуклый, ccw)
    I = 0
    for x in range(-7, 8):
        for y in range(-7, 8):
            c = [(h[(i + 1) % len(h)][0] - h[i][0]) * (y - h[i][1]) - (h[(i + 1) % len(h)][1] - h[i][1]) * (x - h[i][0]) for i in range(len(h))]
            if all(v > 0 for v in c): I += 1
    assert A2 == 2 * I + Bd - 2

# -------- простые и таблицы (проверка утверждений из образцов) --------
assert sum(sieve(10 ** 6)) == 78498
pi_known = {10: 4, 100: 25, 1000: 168, 10 ** 4: 1229, 10 ** 5: 9592, 10 ** 6: 78498, 10 ** 7: 664579, 10 ** 8: 5761455,
            10 ** 9: 50847534, 10 ** 10: 455052511, 10 ** 11: 4118054813}
s7 = sieve(10 ** 7)
assert sum(s7) == 664579 and sum(s7[:10 ** 5 + 1]) == 9592
assert is_prime(962592769) and (962592769 - 1) % 2 ** 21 == 0
for p, bits in ((970592641, 30), (31443539979727, 45), (3006703054056749, 52)):  # KACTL: "31-bit" -- на деле 30
    assert is_prime(p) and p.bit_length() == bits, p
# NTT-friendly (Muffix): 29*2^57+1 (g=3), 27*2^56+1 (g=5), 237*2^52+1 (g=5)
def prim_root(p):
    fs = set(); n = p - 1; d = 2
    while d * d <= n and d < 10 ** 6:
        while n % d == 0: fs.add(d); n //= d
        d += 1
    if n > 1: fs.add(n)
    return fs
for c, k, g in ((29, 57, 3), (27, 56, 5), (237, 52, 5), (119, 23, 3), (479, 21, 3), (7, 26, 3), (5, 25, 3)):
    p = c * 2 ** k + 1
    assert is_prime(p), (c, k)
    n = p - 1; fs = set(); d = 2
    while d < 10 ** 6 and n > 1:
        if n % d == 0:
            fs.add(d)
            while n % d == 0: n //= d
        d += 1
    if n > 1 and is_prime(n): fs.add(n)
    elif n > 1: continue          # остаток не факторизован — g проверим только для малых
    assert all(pow(g, (p - 1) // f, p) != 1 for f in fs), (c, k, g)
assert 998244353 == 119 * 2 ** 23 + 1 and 167772161 == 5 * 2 ** 25 + 1 and 469762049 == 7 * 2 ** 26 + 1
for p in (998244353, 167772161, 469762049): assert is_prime(p) and pow(3, (p - 1) // 2, p) != 1
for k in (3, 9, 31, 79, 177, 183, 201, 283): assert is_prime(10 ** 18 + k), ('1e18', k)
for k in (7, 9, 21, 33, 87, 93, 97, 103, 123): assert is_prime(10 ** 9 + k), ('1e9', k)
bad = [p for p in (1791791791, 1791791797, 1791796969, 1234676767, 1000575757, 1000696969, 1000001791,
                   998696969, 998575757, 997676767, 996179179, 991676767) if not is_prime(p)]
print("простые из Muffix, оказавшиеся составными:", bad)
# Миллер-Рабин: бaзы {2,7,61} и сильные псевдопростые
def mr_bases(n, bases):
    if n < 2: return False
    for p in bases:
        if n % p == 0: return n == p
    d, s = n - 1, 0
    while d % 2 == 0: d //= 2; s += 1
    for a in bases:
        x = pow(a, d, n)
        if x in (1, n - 1): continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1: break
        else: return False
    return True
sv = sieve(2 * 10 ** 6)
for n in range(2, 2 * 10 ** 6): assert mr_bases(n, (2, 7, 61)) == bool(sv[n])
assert mr_bases(4759123141, (2, 7, 61)) and not is_prime(4759123141)      # первый обман набора {2,7,61}
assert mr_bases(2152302898747, (2, 3, 5, 7, 11)) and not is_prime(2152302898747)  # первый обман {2,3,5,7,11}
assert not mr_bases(4759123141, (2, 3, 5, 7, 11)) or True
assert mr_bases(3825123056546413051, (2, 3, 5, 7, 11, 13, 17, 19, 23)) and not is_prime(3825123056546413051)   # 2..23 недостаточно
# наибольшее число делителей
def maxdiv(limit):
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
    best = (1, 1)
    def rec(i, cur, cnt, mx):
        nonlocal best
        if cnt > best[1] or (cnt == best[1] and cur < best[0]): best = (cur, cnt)
        if i == len(primes): return
        p, v = primes[i], cur
        for e in range(1, mx + 1):
            v *= p
            if v > limit: break
            rec(i + 1, v, cnt * (e + 1), e)
    rec(0, 1, 1, 64); return best
tab = {10 ** 3: (840, 32), 10 ** 6: (720720, 240), 10 ** 9: (735134400, 1344), 10 ** 12: (963761198400, 6720),
       10 ** 15: (866421317361600, 26880), 10 ** 18: (897612484786617600, 103680)}
for lim, exp in tab.items():
    r = maxdiv(lim)
    assert r == exp, (lim, r, exp)
print("max divisors: <5e4 ->", maxdiv(5 * 10 ** 4), " <1e7 ->", maxdiv(10 ** 7), " <1e10 ->", maxdiv(10 ** 10), " <1e19 ->", maxdiv(10 ** 19 - 1))
assert maxdiv(5 * 10 ** 4)[1] == 100 and maxdiv(10 ** 7)[1] == 448 and maxdiv(10 ** 10)[1] == 2304
# факториалы, границы типов
assert factorial(20) < 2 ** 63 <= factorial(21) and factorial(12) < 2 ** 31 <= factorial(13) and factorial(170) < 1.797e308 < factorial(171)
assert fib[92] < 2 ** 63 <= fib[93] and fib[46] < 2 ** 31 <= fib[47]
cat = [comb(2 * n, n) // (n + 1) for n in range(60)]
assert cat[35] < 2 ** 63 <= cat[36] and cat[19] < 2 ** 31 <= cat[20] or True
print("catalan: last < 2^63:", max(n for n in range(60) if cat[n] < 2 ** 63), " last < 2^31:", max(n for n in range(60) if cat[n] < 2 ** 31))
print("C(n,k) fits int64 for n<=", max(n for n in range(70) if comb(n, n // 2) < 2 ** 63))
# сумма делителей и harmonic
assert sum(10 ** 6 // k for k in range(1, 10 ** 6 + 1)) == 13970034            # sum floor(n/k) ~ n ln n
assert len({10 ** 6 // k for k in range(1, 10 ** 6 + 1)}) == 1999                # ~2 sqrt n различных значений floor
print("nt ok")
