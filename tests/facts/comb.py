"""Проверка комбинаторных фактов (биномы, Стирлинг, Каталан, подсчёты). Падает на первой ошибке."""
from math import comb, factorial, gcd
from fractions import Fraction
from itertools import permutations, product, combinations
import sys

N = 24
def C(n, k): return comb(n, k) if 0 <= k <= n else 0
fib = [0, 1]
for _ in range(100): fib.append(fib[-1] + fib[-2])

for n in range(N):
    for k in range(n + 1):
        assert sum((-1) ** i * C(n, i) for i in range(k + 1)) == (-1) ** k * C(n - 1, k) if n >= 1 else True
        assert sum(C(n + i, i) for i in range(k + 1)) == C(n + k + 1, k)         # диагональная сумма
    for r in range(n + 1):
        assert sum(C(i, r) for i in range(r, n + 1)) == C(n + 1, r + 1)           # хоккейная клюшка
    assert sum(C(n, k) ** 2 for k in range(n + 1)) == C(2 * n, n)
    assert sum(k * C(n, k) for k in range(n + 1)) == n * 2 ** (n - 1 if n else 0) * (1 if n else 0)
    assert sum(C(n - k, k) for k in range(n + 1)) == fib[n + 1]                   # сумма по "ступеням" = Fib
    assert sum(C(n, k) for k in range(0, n + 1, 2)) == (2 ** (n - 1) if n else 1)
    for m in range(n + 1):
        assert sum(C(n, k) * C(k, m) for k in range(n + 1)) == C(n, m) * 2 ** (n - m)
        for r in range(n + 1):
            assert sum(C(m, k) * C(n, r - k) for k in range(r + 1)) == C(m + n, r)   # Вандермонд
# отрицательный верхний индекс: C(-n,k) = (-1)^k C(n+k-1,k)
def Cg(a, k):
    num = 1
    for i in range(k): num *= (a - i)
    return Fraction(num, factorial(k))
for n in range(1, 10):
    for k in range(10):
        assert Cg(-n, k) == (-1) ** k * C(n + k - 1, k)
# отношение биномов: sum C(k,i)/C(n,i) = C(n+1,n-k+1)/C(n,k)
for n in range(1, 15):
    for k in range(n + 1):
        assert sum(Fraction(C(k, i), C(n, i)) for i in range(k + 1)) == Fraction(C(n + 1, n - k + 1), C(n, k))
# Куммер: v_p(C(n,m)) = число переносов при сложении m и n-m в системе p
def vp(x, p):
    c = 0
    while x % p == 0: x //= p; c += 1
    return c
def carries(a, b, p):
    c = cr = 0
    while a or b or cr:
        s = a % p + b % p + cr
        cr = 1 if s >= p else 0
        c += cr
        a //= p; b //= p
    return c
for p in (2, 3, 5, 7):
    for n in range(0, 60):
        for m in range(n + 1):
            assert vp(C(n, m), p) == carries(m, n - m, p)
# Лукас
def lucas(n, k, p):
    r = 1
    while n or k:
        r = r * C(n % p, k % p) % p
        n //= p; k //= p
    return r
for p in (2, 3, 5, 7, 11):
    for n in range(80):
        for k in range(n + 1): assert lucas(n, k, p) == C(n, k) % p
# шарики и перегородки с ограничением сверху
def bounded(n, m, k):
    return sum((-1) ** t * C(n, t) * C(m - t * (k + 1) + n - 1, n - 1) for t in range(0, min(n, m // (k + 1)) + 1))
for n in range(1, 5):
    for m in range(0, 10):
        for k in range(0, 6):
            br = sum(1 for a in product(range(k + 1), repeat=n) if sum(a) == m)
            assert bounded(n, m, k) == br
            assert sum(1 for a in product(range(m + 1), repeat=n) if sum(a) == m) == C(m + n - 1, n - 1)
            if m >= n: assert sum(1 for a in product(range(1, m + 1), repeat=n) if sum(a) == m) == C(m - 1, n - 1)
# производящие: (1-x)^-n = sum C(n+k-1,n-1) x^k — выше; суммы степеней
for n in range(0, 30):
    assert sum(i ** 2 for i in range(n + 1)) == n * (n + 1) * (2 * n + 1) // 6
    assert sum(i ** 3 for i in range(n + 1)) == (n * (n + 1) // 2) ** 2
    assert sum(i ** 4 for i in range(n + 1)) == n * (n + 1) * (2 * n + 1) * (3 * n * n + 3 * n - 1) // 30
for a in range(2, 6):
    for n in range(1, 12):
        assert sum(i * a ** i for i in range(1, n + 1)) * (a - 1) ** 2 == a * (n * a ** (n + 1) - (n + 1) * a ** n + 1)

# числа Стирлинга, Белла, Эйлера
M = 14
s1 = [[0] * (M + 1) for _ in range(M + 1)]; s2 = [[0] * (M + 1) for _ in range(M + 1)]; eu = [[0] * (M + 1) for _ in range(M + 1)]
s1[0][0] = s2[0][0] = 1
for n in range(1, M + 1):
    for k in range(1, n + 1):
        s1[n][k] = s1[n - 1][k - 1] + (n - 1) * s1[n - 1][k]       # беззнаковые
        s2[n][k] = s2[n - 1][k - 1] + k * s2[n - 1][k]
for n in range(1, M + 1):
    for k in range(n):
        eu[n][k] = 1 if k == 0 else (n - k) * eu[n - 1][k - 1] + (k + 1) * eu[n - 1][k]
        if n == 1: eu[n][0] = 1
        assert eu[n][k] == sum((-1) ** j * C(n + 1, j) * (k + 1 - j) ** n for j in range(k + 2)) if True else 0
for n in range(0, M + 1):
    for k in range(0, n + 1):
        assert s2[n][k] * factorial(k) == sum((-1) ** (k - j) * C(k, j) * j ** n for j in range(k + 1))
    assert sum(eu[n][:n]) == factorial(n) if n else True
    # убывающие/возрастающие степени: x^n = sum S(n,k) x_(k);  x^(n rising) = sum c(n,k) x^k
    for x in range(0, 6):
        assert x ** n == sum(s2[n][k] * (factorial(x) // factorial(x - k) if k <= x else 0) for k in range(n + 1))
        rise = 1
        for i in range(n): rise *= (x + i)
        assert rise == sum(s1[n][k] * x ** k for k in range(n + 1))
bell = [sum(s2[n][k] for k in range(n + 1)) for n in range(M)]
assert bell[:13] == [1, 1, 2, 5, 15, 52, 203, 877, 4140, 21147, 115975, 678570, 4213597]
for n in range(1, M):
    assert bell[n] == sum(C(n - 1, k - 1) * bell[n - k] for k in range(1, n + 1))
# sum C(n,i) i^k
for n in range(0, 10):
    for k in range(0, 7):
        assert sum(C(n, i) * i ** k for i in range(n + 1)) == sum(s2[k][j] * (factorial(n) // factorial(n - j) if j <= n else 0) * 2 ** (n - j) for j in range(0, min(k, n) + 1))
# приведённые числа Стирлинга: разбиения, где внутри блока элементы различаются не менее чем на d
def parts(n):
    def rec(i, blocks):
        if i > n: yield [list(b) for b in blocks]; return
        for b in blocks:
            b.append(i); yield from rec(i + 1, blocks); b.pop()
        blocks.append([i]); yield from rec(i + 1, blocks); blocks.pop()
    yield from rec(1, [])
for n in range(1, 9):
    for d in range(1, 5):
        for k in range(d, n + 1):  # формула верна при k >= d
            cnt = sum(1 for p in parts(n) if len(p) == k and all(b[i + 1] - b[i] >= d for b in p for i in range(len(b) - 1)))
            exp = s2[n - d + 1][k - d + 1] if n - d + 1 >= 0 and k - d + 1 >= 0 else 0
            if n - d + 1 == 0 and k - d + 1 == 0: exp = 1
            assert cnt == exp, (n, d, k, cnt, exp)
# перестановки с m циклами
for n in range(1, 8):
    cyc = {}
    for p in permutations(range(n)):
        seen, c = [False] * n, 0
        for i in range(n):
            if not seen[i]:
                c += 1; j = i
                while not seen[j]: seen[j] = True; j = p[j]
        cyc[c] = cyc.get(c, 0) + 1
    for k in range(1, n + 1): assert cyc.get(k, 0) == s1[n][k]
    assert sum(Fraction(k * v, 1) for k, v in cyc.items()) / factorial(n) == sum(Fraction(1, t) for t in range(1, n + 1))   # E[циклов] = H_n
# беспорядки
d = [1, 0]
for n in range(2, 20): d.append((n - 1) * (d[n - 1] + d[n - 2]))
for n in range(0, 20):
    assert d[n] == sum((-1) ** t * C(n, t) * factorial(n - t) for t in range(n + 1))
    if n: assert d[n] == n * d[n - 1] + (-1) ** n
    if 1 <= n < 15: assert d[n] == round(factorial(n) / 2.718281828459045)
    if n >= 1: assert sum(C(n, k) * d[n - k] for k in range(n + 1)) == factorial(n)
assert d[:9] == [1, 0, 1, 2, 9, 44, 265, 1854, 14833]
# Каталан, Нараяна, ПСП
cat = [C(2 * n, n) // (n + 1) for n in range(20)]
assert cat[:11] == [1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796]
for n in range(1, 18):
    assert cat[n] == C(2 * n, n) - C(2 * n, n - 1) == C(2 * n + 1, n) // (2 * n + 1)
    assert cat[n] == sum(cat[i] * cat[n - 1 - i] for i in range(n))
    for k in range(1, n + 1): assert Fraction(C(n, k) * C(n, k - 1), n).denominator == 1
    assert sum(C(n, k) * C(n, k - 1) // n for k in range(1, n + 1)) == cat[n]
def psp(n):
    res = []
    def rec(s, o, c):
        if o == n and c == n: res.append(s); return
        if o < n: rec(s + '(', o + 1, c)
        if c < o: rec(s + ')', o, c + 1)
    rec('', 0, 0); return res
for n in range(1, 10):
    ps = psp(n); assert len(ps) == cat[n]
    for k in range(1, n + 1):
        assert sum(1 for s in ps if s.count('()') == k) == C(n, k) * C(n, k - 1) // n   # k пиков "()" -> Нараяна
    assert len(ps) == cat[n]
# неполные ПСП: n открывающих, m<=n закрывающих, префикс-баланс >= 0
for n in range(0, 9):
    for m in range(0, n + 1):
        cnt = 0
        for pos in combinations(range(n + m), m):
            s = [1] * (n + m)
            for p in pos: s[p] = -1
            b, ok = 0, True
            for x in s:
                b += x
                if b < 0: ok = False; break
            cnt += ok
        assert cnt == C(n + m, m) - C(n + m, m - 1)
# свёртка Каталана: sum_{x0+..+xk=n} C_x0..C_xk = (k+1)/(n+k+1) C(2n+k, n)
for k in range(0, 4):
    for n in range(0, 8):
        tot = 0
        for xs in product(range(n + 1), repeat=k + 1):
            if sum(xs) == n:
                pr = 1
                for x in xs: pr *= cat[x]
                tot += pr
        assert tot * (n + k + 1) == (k + 1) * C(2 * n + k, n)
# число триангуляций выпуклого (n+2)-угольника = Cn; количество деревьев Кэли и лесов
for n in range(1, 9):
    pass  # проверка Кэли/лесов — в graph.py
# Бернсайд для ожерелий (повороты): (1/n) sum_{d|n} phi(d) k^{n/d}
def phi(n): return sum(1 for i in range(1, n + 1) if gcd(i, n) == 1)
for n in range(1, 9):
    for k in range(1, 4):
        seen = set()
        for s in product(range(k), repeat=n):
            seen.add(min(s[i:] + s[:i] for i in range(n)))
        assert len(seen) == sum(phi(dv) * k ** (n // dv) for dv in range(1, n + 1) if n % dv == 0) // n
# разбиения числа: пентагональная формула и таблица
p = [1]
for n in range(1, 101):
    s = 0; k = 1
    while True:
        g1, g2 = k * (3 * k - 1) // 2, k * (3 * k + 1) // 2
        if g1 > n: break
        sg = 1 if k % 2 else -1
        s += sg * p[n - g1] + (sg * p[n - g2] if g2 <= n else 0)
        k += 1
    p.append(s)
assert p[:10] == [1, 1, 2, 3, 5, 7, 11, 15, 22, 30] and p[20] == 627 and p[50] == 204226 and p[100] == 190569292
# число решений a1+...+ak=n в положительных (композиции) 2^(n-1); ветви
# "число бинарных строк с ровно m различными подпоследовательностями (с пустой) = phi(m+1)" -- из Muffix
def distinct_subseq(s):
    seen = {''}
    for ch in s:
        seen |= {x + ch for x in seen}
    return len(seen)
cnt = {}
for L in range(0, 13):
    for s in product('01', repeat=L):
        m = distinct_subseq(''.join(s))
        cnt[m] = cnt.get(m, 0) + 1
for m in range(1, 12):  # строки длины > m-1 не бывают, так как дают >= L+1 подпоследовательностей
    assert cnt[m] == phi(m + 1), (m, cnt[m], phi(m + 1))
print("comb ok")
