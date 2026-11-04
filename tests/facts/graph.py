"""Графовые теоремы: проверка на малых графах перебором."""
from itertools import combinations, product, permutations
from fractions import Fraction
from math import factorial
import random
random.seed(7)

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
def comps(n, edges):
    p = list(range(n))
    def f(x):
        while p[x] != x: p[x] = p[p[x]]; x = p[x]
        return x
    for a, b in edges: p[f(a)] = f(b)
    return len({f(i) for i in range(n)})

# Кэли: n^(n-2) остовов K_n (Кирхгоф) и лес из k компонент с k выделенными корнями: k n^(n-k-1)
for n in range(2, 8):
    L = [[(n - 1 if i == j else -1) for j in range(n)] for i in range(n)]
    assert det([r[1:] for r in L[1:]]) == n ** (n - 2)
for n in range(2, 7):
    alle = list(combinations(range(n), 2))
    for k in range(1, n + 1):
        cnt = 0
        for m in range(n - k, n - k + 1):                 # лес с k компонентами имеет n-k рёбер
            for es in combinations(alle, m):
                if comps(n, es) == k and comps(n, list(es) + [(i, j) for i in range(k) for j in range(i)]) == 1:
                    # вершины 0..k-1 в разных компонентах <=> после слияния всех них компонента одна и не возникает цикла
                    pass
        # проще: считаем леса, где вершины 0..k-1 лежат в разных деревьях
        for es in combinations(alle, n - k):
            if comps(n, es) != k: continue
            if len({root for root in range(k)}) == 0: continue
            p = list(range(n))
            def f(x):
                while p[x] != x: x = p[x]
                return x
            for a, b in es: p[f(a)] = f(b)
            if len({f(i) for i in range(k)}) == k: cnt += 1
        assert cnt == k * n ** (n - k - 1) if k < n else cnt == 1, (n, k, cnt)
# сколько всего лесов с k компонентами (без выделения) -- НЕ k n^(n-k-1): проверка расхождения
n, k = 4, 2
tot = sum(1 for es in combinations(list(combinations(range(n), 2)), n - k) if comps(n, es) == k)
assert tot != k * n ** (n - k - 1)

# Кирхгоф для ориентированных: деревья, направленные ОТ корня, = минор L = D_in - A
def count_arbor(n, edges, root):
    cnt = 0
    others = [v for v in range(n) if v != root]
    inc = {v: [e for e in edges if e[1] == v] for v in others}
    for ch in product(*[inc[v] for v in others]):
        par = {v: e[0] for v, e in zip(others, ch)}
        ok = True
        for v in others:
            x, st = v, 0
            while x != root and st <= n: x = par[x]; st += 1
            if x != root: ok = False; break
        cnt += ok
    return cnt
for _ in range(60):
    n = random.randint(2, 5)
    edges = [(random.randrange(n), random.randrange(n)) for _ in range(random.randint(1, 9))]
    edges = [e for e in edges if e[0] != e[1]]
    L = [[0] * n for _ in range(n)]
    for u, v in edges: L[u][v] -= 1; L[v][v] += 1                   # так в KACTL / у учителя
    r = random.randrange(n)
    minor = [[L[i][j] for j in range(n) if j != r] for i in range(n) if i != r]
    assert det(minor) == count_arbor(n, edges, r)

# BEST: ec(G) = t_w(G) * prod (deg(v)-1)!  (эйлеровы циклы как циклические последовательности рёбер)
def euler_circuits(n, edges):
    m = len(edges); out = {v: [i for i, e in enumerate(edges) if e[0] == v] for v in range(n)}
    start = edges[0][0]; seen = set()
    def rec(v, used, seq):
        if len(seq) == m:
            if v == start: seen.add(tuple(seq))
            return
        for i in out[v]:
            if i not in used:
                used.add(i); seq.append(i); rec(edges[i][1], used, seq); seq.pop(); used.discard(i)
    # циклический порядок рёбер: начинаем с фиксированного ребра 0 (оно входит в цикл)
    first = 0
    def rec2(v, used, seq):
        if len(seq) == m:
            if v == edges[first][0]: seen.add(tuple(seq))
            return
        for i in out[v]:
            if i not in used:
                used.add(i); seq.append(i); rec2(edges[i][1], used, seq); seq.pop(); used.discard(i)
    rec2(edges[first][1], {first}, [first])
    return len(seen)
tested = 0
for _ in range(400):
    n = random.randint(2, 4)
    edges = [(random.randrange(n), random.randrange(n)) for _ in range(random.randint(2, 7))]
    indeg = [sum(1 for e in edges if e[1] == v) for v in range(n)]; outd = [sum(1 for e in edges if e[0] == v) for v in range(n)]
    if indeg != outd or comps(n, [e for e in edges]) != 1 + sum(1 for v in range(n) if outd[v] == 0): continue
    verts = [v for v in range(n) if outd[v] > 0]
    L = [[0] * n for _ in range(n)]
    for u, v in edges: L[u][v] -= 1; L[u][u] += 1                     # out-Laplacian: деревья, направленные К корню
    w = verts[0]
    minor = [[L[i][j] for j in verts if j != w] for i in verts if i != w]
    t = det(minor) if minor else 1
    best = t
    for v in verts: best *= factorial(outd[v] - 1)
    assert euler_circuits(n, edges) == best, (edges,)
    tested += 1
assert tested > 20

# Эрдёш–Галлаи = Хавел–Хакими = перебор графов
def eg(d):
    d = sorted(d, reverse=True); n = len(d)
    if sum(d) % 2: return False
    return all(sum(d[:k]) <= k * (k - 1) + sum(min(x, k) for x in d[k:]) for k in range(1, n + 1))
def hh(d):
    d = sorted(d, reverse=True)
    while d:
        if d[0] < 0 or d[0] > len(d) - 1: return False
        x = d.pop(0)
        for i in range(x): d[i] -= 1
        d.sort(reverse=True)
        if d and d[-1] < 0: return False
    return True
for n in range(1, 7):
    alle = list(combinations(range(n), 2)); gset = set()
    for mask in range(1 << len(alle)):
        deg = [0] * n
        for i, (a, b) in enumerate(alle):
            if mask >> i & 1: deg[a] += 1; deg[b] += 1
        gset.add(tuple(sorted(deg, reverse=True)))
    for d in product(range(n), repeat=n):
        if list(d) != sorted(d, reverse=True): continue
        assert eg(list(d)) == (d in gset) == hh(list(d)), d

# Кёниг, Галлаи, Дилворт, Хол
def max_matching(L, R, edges):
    adj = {l: [r for (a, r) in edges if a == l] for l in range(L)}
    mt = [-1] * R
    def dfs(v, vis):
        for u in adj[v]:
            if u in vis: continue
            vis.add(u)
            if mt[u] == -1 or dfs(mt[u], vis): mt[u] = v; return True
        return False
    return sum(dfs(v, set()) for v in range(L))
for _ in range(300):
    L, R = random.randint(1, 5), random.randint(1, 5)
    edges = [(a, b) for a in range(L) for b in range(R) if random.random() < 0.4]
    mm = max_matching(L, R, edges)
    cover = min(len(c) for k in range(L + R + 1) for c in combinations(range(L + R), k)
                if all(a in c or L + b in c for a, b in edges))[0:0] if False else None
    best_cover = min(k for k in range(L + R + 1) if any(all(a in c or L + b in c for a, b in edges) for c in combinations(range(L + R), k)))
    assert best_cover == mm                                         # Кёниг
    best_is = max(k for k in range(L + R + 1) if any(all(not (a in c and L + b in c) for a, b in edges) for c in combinations(range(L + R), k)))
    assert best_is == L + R - mm                                    # дополнение покрытия = независимое
    # Хол
    hall = all(len({b for (a, b) in edges if a in S}) >= len(S) for k in range(L + 1) for S in map(set, combinations(range(L), k)))
    assert hall == (mm == L)
    # минимальное рёберное покрытие (если нет изолированных) = n - паросочетание (Галлаи) -- для двудольного
    if all(any(a == v for a, b in edges) for v in range(L)) and all(any(b == u for a, b in edges) for u in range(R)):
        best_ec = min(k for k in range(len(edges) + 1) if any({x for (a, b) in c for x in (a, L + b)} == set(range(L + R)) for c in combinations(edges, k)))
        assert best_ec == L + R - mm
for _ in range(120):                                                # Дилворт: максимальная антицепь = min покрытие цепями = n - паросочетание в транз. замыкании
    n = random.randint(1, 7)
    lt = [[False] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if random.random() < 0.35: lt[i][j] = True
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if lt[i][k] and lt[k][j]: lt[i][j] = True
    edges = [(i, j) for i in range(n) for j in range(n) if lt[i][j]]
    width = max(len(c) for k in range(n + 1) for c in combinations(range(n), k) if all(not lt[a][b] and not lt[b][a] for a, b in combinations(c, 2))) if n else 0
    assert width == n - max_matching(n, n, edges)
# Татт–Берж: nu(G) = min_U (|V| + |U| - odd(G-U)) / 2
def oddcomps(n, edges, removed):
    vs = [v for v in range(n) if v not in removed]; p = {v: v for v in vs}
    def f(x):
        while p[x] != x: x = p[x]
        return x
    for a, b in edges:
        if a in p and b in p: p[f(a)] = f(b)
    cnt = {}
    for v in vs: cnt[f(v)] = cnt.get(f(v), 0) + 1
    return sum(1 for c in cnt.values() if c % 2)
def brute_matching(n, edges):
    best = 0
    def rec(i, used, m):
        nonlocal best
        best = max(best, m)
        for j in range(i, len(edges)):
            a, b = edges[j]
            if a not in used and b not in used: rec(j + 1, used | {a, b}, m + 1)
    rec(0, frozenset(), 0); return best
for _ in range(200):
    n = random.randint(1, 8)
    edges = [e for e in combinations(range(n), 2) if random.random() < 0.3]
    tb = min((n + len(U) - oddcomps(n, edges, set(U))) // 2 for k in range(n + 1) for U in combinations(range(n), k) if (n + len(U) - oddcomps(n, edges, set(U))) % 2 == 0)
    assert tb == brute_matching(n, edges)
# хроматические многочлены
def colorings(n, edges, t):
    return sum(1 for c in product(range(t), repeat=n) if all(c[a] != c[b] for a, b in edges))
for t in range(0, 5):
    for n in range(1, 6):
        path = [(i, i + 1) for i in range(n - 1)]; cyc = path + [(n - 1, 0)] if n >= 3 else None
        assert colorings(n, path, t) == t * (t - 1) ** (n - 1)
        assert colorings(n, list(combinations(range(n), 2)), t) == (lambda: __import__('math').perm(t, n))()
        if cyc: assert colorings(n, cyc, t) == (t - 1) ** n + (-1) ** n * (t - 1)
print("graph ok")

# Дилворт: построение антицепи по покрытию Кёнига и покрытие DAG путями (без замыкания)
for _ in range(150):
    n = random.randint(1, 6)
    lt = [[False] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if random.random() < 0.4: lt[i][j] = True
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if lt[i][k] and lt[k][j]: lt[i][j] = True
    edges = [(i, j) for i in range(n) for j in range(n) if lt[i][j]]
    cov = None
    for k in range(2 * n + 1):
        for c in combinations(range(2 * n), k):
            if all(a in c or n + b in c for a, b in edges): cov = set(c); break
        if cov is not None: break
    anti = [v for v in range(n) if v not in cov and n + v not in cov]
    assert all(not lt[a][b] and not lt[b][a] for a, b in combinations(anti, 2))
    assert len(anti) == n - max_matching(n, n, edges) or len(anti) >= n - max_matching(n, n, edges)
    assert len(anti) == n - len(cov)
for _ in range(150):                                         # путь-покрытие DAG непересекающимися путями = n - паросочетание
    n = random.randint(1, 6)
    edges = [(i, j) for i in range(n) for j in range(i + 1, n) if random.random() < 0.35]
    best = n
    for mask in range(1 << len(edges)):
        sel = [edges[i] for i in range(len(edges)) if mask >> i & 1]
        if len({a for a, b in sel}) == len(sel) and len({b for a, b in sel}) == len(sel): best = min(best, n - len(sel))
    assert best == n - max_matching(n, n, edges)
print("graph2 ok")
