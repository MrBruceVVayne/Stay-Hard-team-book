"""Комбинаторные утверждения с сайта mangooste.ru и из Muffix, не вошедшие в comb.py."""
from math import comb, factorial
from fractions import Fraction
from itertools import product, combinations
C = lambda n, k: comb(n, k) if 0 <= k <= n else 0

# непересекающиеся пары путей (0,0)->(n,m), общих точек только концы; неупорядоченные пары
def paths(n, m):
    res = []
    for ups in combinations(range(n + m), m):
        p = [(0, 0)]; x = y = 0
        for i in range(n + m):
            if i in ups: y += 1
            else: x += 1
            p.append((x, y))
        res.append(p)
    return res
for n in range(1, 6):
    for m in range(1, 6):
        ps = paths(n, m); cnt = 0
        for i in range(len(ps)):
            for j in range(i + 1, len(ps)):
                if set(ps[i]) & set(ps[j]) == {(0, 0), (n, m)}: cnt += 1
        assert cnt == C(n + m - 2, n - 1) ** 2 - C(n + m - 2, n) * C(n + m - 2, m), (n, m, cnt)
# распределение n различимых людей по k группам размера <= m (группы различимы, могут быть пустыми)
from functools import lru_cache
def f_rec(n, k, m):
    @lru_cache(None)
    def f(n, k):
        if n == 0: return 1
        if n < 0 or k == 0: return 0
        return k * f(n - 1, k) - k * C(n - 1, m) * f(n - m - 1, k - 1)
    return f(n, k)
for m in range(1, 4):
    for k in range(1, 4):
        for n in range(0, 8):
            brute = sum(1 for a in product(range(k), repeat=n) if all(a.count(g) <= m for g in range(k)))
            assert f_rec(n, k, m) == brute, (n, k, m)
# деревья с заданными степенями: (n-2)!/prod (d_v-1)!  (перебор через коды Прюфера)
for n in range(2, 7):
    for d in product(range(1, n), repeat=n):
        if sum(d) != 2 * (n - 1): continue
        cnt = sum(1 for code in product(range(n), repeat=n - 2) if all(code.count(v) == d[v] - 1 for v in range(n)))
        pr = 1
        for x in d: pr *= factorial(x - 1)
        assert cnt * pr == factorial(n - 2)
# скользящая сумма биномов B(l,r,n): 2B(l,r,n) = C(n,l)+C(n,r)-C(n+1,l)+B(l,r,n+1)
B = lambda l, r, n: sum(C(n, i) for i in range(l, r + 1))
ok_as_written = all(2 * B(l, r, n) == C(n, l) + C(n, r) - C(n + 1, l) + B(l, r, n + 1) for n in range(20) for l in range(8) for r in range(l, 9))
print("Muffix rolling-range-sum as written holds:", ok_as_written)
# верная форма: B(l,r,n+1) = 2B(l,r,n) + C(n,l-1) - C(n,r)
assert all(B(l, r, n + 1) == 2 * B(l, r, n) + C(n, l - 1) - C(n, r) for n in range(25) for l in range(0, 8) for r in range(l, 10))
# сумма i^j-взвешенных C(i,j) x^i (Muffix)
for n in range(1, 8):
    for j in range(0, 5):
        for x in (Fraction(2), Fraction(3), Fraction(-2), Fraction(1, 3)):
            if x == 1: continue
            lhs = sum(C(i, j) * x ** i for i in range(n))
            rhs = x ** j * (1 - x) ** (-j - 1) * (1 - x ** (n - j) * sum(C(n, i) * x ** (j - i) * (1 - x) ** i for i in range(j + 1))) if n >= j else None
            if rhs is not None: assert lhs == rhs, (n, j, x)
# p-я скобка закрывающая в случайной ПСП длины 2n (mangooste)
cat = [C(2 * n, n) // (n + 1) for n in range(12)]
def psps(n):
    out = []
    def rec(s, o, c):
        if len(s) == 2 * n: out.append(s); return
        if o < n: rec(s + '(', o + 1, c)
        if c < o: rec(s + ')', o, c + 1)
    rec('', 0, 0); return out
for n in range(1, 8):
    ps = psps(n)
    for p in range(2 * n):
        cnt = sum(1 for s in ps if s[p] == ')')
        assert cnt == sum(cat[m] * cat[n - m - 1] for m in range(0, min(n - 1, (p - 1) // 2) + 1)) if p >= 1 else cnt == 0, (n, p)
# число ПСП из 2n со ровно x пиками "()" -- проверено в comb.py; ПСП с k блоками = Нараяна
# коды Прюфера: биекция
for n in range(3, 6):
    codes = set()
    for code in product(range(n), repeat=n - 2):
        deg = [1] * n
        for c in code: deg[c] += 1
        edges = []
        cd = list(code)
        for c in cd:
            leaf = min(v for v in range(n) if deg[v] == 1)
            edges.append((leaf, c)); deg[leaf] -= 1; deg[c] -= 1
        u, v = [x for x in range(n) if deg[x] == 1]; edges.append((u, v))
        codes.add(frozenset(frozenset(e) for e in edges))
    assert len(codes) == n ** (n - 2)
print("comb2 ok")
