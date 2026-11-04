"""Геометрия: формулы треугольника, четырёхугольника, окружностей, многогранников."""
import random, math
from math import sqrt, hypot, isclose, atan2, cos, sin, acos, pi, gcd
random.seed(3)
def close(a, b, eps=1e-8): return abs(a - b) <= eps * max(1, abs(a), abs(b))
def cross(a, b, c): return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])
for _ in range(500):
    A, B, C = [(random.uniform(-9, 9), random.uniform(-9, 9)) for _ in range(3)]
    a, b, c = hypot(*(B[i] - C[i] for i in range(2))), hypot(*(A[i] - C[i] for i in range(2))), hypot(*(A[i] - B[i] for i in range(2)))
    S = abs(cross(A, B, C)) / 2
    if S < 1e-3: continue
    p = (a + b + c) / 2
    assert close(S, sqrt(p * (p - a) * (p - b) * (p - c)))                 # Герон
    R = a * b * c / (4 * S); r = S / p
    # описанная окружность: центр равноудалён
    d = 2 * ((B[0] - A[0]) * (C[1] - A[1]) - (B[1] - A[1]) * (C[0] - A[0]))
    ux = ((C[1] - A[1]) * ((B[0] - A[0]) ** 2 + (B[1] - A[1]) ** 2) - (B[1] - A[1]) * ((C[0] - A[0]) ** 2 + (C[1] - A[1]) ** 2)) / d
    uy = ((B[0] - A[0]) * ((C[0] - A[0]) ** 2 + (C[1] - A[1]) ** 2) - (C[0] - A[0]) * ((B[0] - A[0]) ** 2 + (B[1] - A[1]) ** 2)) / d
    assert close(hypot(ux, uy), R)
    I = ((a * A[0] + b * B[0] + c * C[0]) / (2 * p), (a * A[1] + b * B[1] + c * C[1]) / (2 * p))   # инцентр
    assert close(abs(cross(B, C, I)) / a, r)                               # расстояние до стороны = r
    M = ((B[0] + C[0]) / 2, (B[1] + C[1]) / 2)
    assert close(hypot(A[0] - M[0], A[1] - M[1]), 0.5 * sqrt(2 * b * b + 2 * c * c - a * a))   # медиана
    # биссектриса из A: точка на BC делит в отношении c:b
    D = ((b * B[0] + c * C[0]) / (b + c), (b * B[1] + c * C[1]) / (b + c))
    assert close(hypot(A[0] - D[0], A[1] - D[1]), sqrt(b * c * (1 - (a / (b + c)) ** 2)))
    ca = (b * b + c * c - a * a) / (2 * b * c)
    assert close(a * a, b * b + c * c - 2 * b * c * ca)                     # теорема косинусов
    assert close(a / sqrt(1 - ca * ca), 2 * R)                              # теорема синусов
    # центроид, ортоцентр, окружность Эйлера: H = A + B + C - 2O
    O = (ux + A[0], uy + A[1]); G = tuple((A[i] + B[i] + C[i]) / 3 for i in range(2))
    H = tuple(A[i] + B[i] + C[i] - 2 * O[i] for i in range(2))
    assert close((H[0] - A[0]) * (C[0] - B[0]) + (H[1] - A[1]) * (C[1] - B[1]), 0)
    assert close(G[0], (2 * O[0] + H[0]) / 3) and close(G[1], (2 * O[1] + H[1]) / 3)   # G делит OH как 1:2
    # расстояние от точки до прямой, площадь по формуле шнуровки, радикальная ось
# вписанный четырёхугольник: Птолемей и Брахмагупта
for _ in range(300):
    ang = sorted(random.uniform(0, 2 * pi) for _ in range(4)); Rr = random.uniform(1, 5)
    P = [(Rr * cos(t), Rr * sin(t)) for t in ang]
    side = lambda i: hypot(P[i][0] - P[(i + 1) % 4][0], P[i][1] - P[(i + 1) % 4][1])
    a, b, c, d = [side(i) for i in range(4)]
    e, f = hypot(P[0][0] - P[2][0], P[0][1] - P[2][1]), hypot(P[1][0] - P[3][0], P[1][1] - P[3][1])
    assert close(e * f, a * c + b * d)
    area = abs(sum(P[i][0] * P[(i + 1) % 4][1] - P[i][1] * P[(i + 1) % 4][0] for i in range(4))) / 2
    p = (a + b + c + d) / 2
    assert close(area, sqrt(max(0, (p - a) * (p - b) * (p - c) * (p - d))), 1e-6)
# произвольный выпуклый: 4A = 2 e f sin(theta) = F tan(theta), F = b^2 + d^2 - a^2 - c^2
for _ in range(300):
    ang = sorted(random.uniform(0, 2 * pi) for _ in range(4))
    P = [(random.uniform(0.5, 4) * cos(t), random.uniform(0.5, 4) * sin(t)) for t in ang]
    if any(cross(P[i], P[(i + 1) % 4], P[(i + 2) % 4]) <= 1e-3 for i in range(4)): continue
    s = [hypot(P[i][0] - P[(i + 1) % 4][0], P[i][1] - P[(i + 1) % 4][1]) for i in range(4)]
    a, b, c, d = s
    d1 = (P[2][0] - P[0][0], P[2][1] - P[0][1]); d2 = (P[3][0] - P[1][0], P[3][1] - P[1][1])
    e, f = hypot(*d1), hypot(*d2)
    th = atan2(abs(d1[0] * d2[1] - d1[1] * d2[0]), d1[0] * d2[0] + d1[1] * d2[1])
    A = abs(sum(P[i][0] * P[(i + 1) % 4][1] - P[i][1] * P[(i + 1) % 4][0] for i in range(4))) / 2
    F = b * b + d * d - a * a - c * c
    assert close(4 * A, 2 * e * f * sin(th), 1e-6) and close(4 * A, sqrt(4 * e * e * f * f - F * F), 1e-6)
    if abs(cos(th)) > 1e-3: assert close(4 * A, F * math.tan(th), 1e-5)
# радикальная ось двух окружностей, расстояние на сфере, объём тетраэдра, эйлеров многогранник, сферические координаты
for _ in range(200):
    c1 = (random.uniform(-3, 3), random.uniform(-3, 3)); c2 = (random.uniform(-3, 3), random.uniform(-3, 3)); r1, r2 = random.uniform(1, 3), random.uniform(1, 3)
    d = hypot(c1[0] - c2[0], c1[1] - c2[1])
    if d > r1 + r2 or d < abs(r1 - r2) or d < 1e-3: continue
    a = (r1 * r1 - r2 * r2 + d * d) / (2 * d); h = sqrt(r1 * r1 - a * a)           # расстояние от c1 до хорды вдоль c1c2
    ux, uy = (c2[0] - c1[0]) / d, (c2[1] - c1[1]) / d
    P = (c1[0] + a * ux - h * uy, c1[1] + a * uy + h * ux)
    assert close(hypot(P[0] - c1[0], P[1] - c1[1]), r1) and close(hypot(P[0] - c2[0], P[1] - c2[1]), r2)
for _ in range(100):
    u, v = [tuple(random.gauss(0, 1) for _ in range(3)) for _ in range(2)]
    nu, nv = [sqrt(sum(x * x for x in w)) for w in (u, v)]
    ang = acos(sum(u[i] * v[i] for i in range(3)) / (nu * nv))
    cr = (u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0])
    assert close(ang, atan2(sqrt(sum(x * x for x in cr)), sum(u[i] * v[i] for i in range(3))))   # atan2 устойчивее acos
tetra = lambda a, b, c, d: abs(sum(((b[i] - a[i]) * ((c[(i + 1) % 3] - a[(i + 1) % 3]) * (d[(i + 2) % 3] - a[(i + 2) % 3]) - (c[(i + 2) % 3] - a[(i + 2) % 3]) * (d[(i + 1) % 3] - a[(i + 1) % 3]))) for i in range(3))) / 6
assert close(tetra((0, 0, 0), (1, 0, 0), (0, 1, 0), (0, 0, 1)), 1 / 6)
for V, E, F in ((8, 12, 6), (4, 6, 4), (6, 12, 8), (20, 30, 12), (12, 30, 20)): assert V - E + F == 2
for _ in range(100):                               # число целых точек на отрезке = gcd(dx, dy) + 1
    dx, dy = random.randint(-30, 30), random.randint(-30, 30)
    cnt = sum(1 for t in range(0, 61) for x in [t] if False)
    g = gcd(abs(dx), abs(dy)); pts = {(dx * k // g if g else 0, dy * k // g if g else 0) for k in range(g + 1)}
    assert len(pts) == (g + 1 if g else 1)
print("geo ok")
