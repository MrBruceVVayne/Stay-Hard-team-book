#pragma once
#include "geoma/point2d.cpp"

// Удвоенная ориентированная площадь (> 0 для обхода против часовой)
template <typename T>
T area2(const ve<mpt<T>> &p) {
        T s = 0;
        for (size_t i = 0; i < p.size(); i++) s += p[i] % p[(i + 1) % p.size()];
        return s;
}

// Выпуклая оболочка (Эндрю), ccw, без коллинеарных точек. Дубликаты убираются.
template <typename T>
ve<mpt<T>> hull(ve<mpt<T>> a) {
        sort(all(a));
        a.erase(unique(all(a)), a.end());
        int n = a.size();
        if (n < 3) return a;
        ve<mpt<T>> h(2 * n);
        int k = 0;
        for (int i = 0; i < n; i++) {
                while (k >= 2 && sgn((h[k - 1] - h[k - 2]) % (a[i] - h[k - 1])) <= 0) k--;
                h[k++] = a[i];
        }
        for (int i = n - 2, t = k + 1; i >= 0; i--) {
                while (k >= t && sgn((h[k - 1] - h[k - 2]) % (a[i] - h[k - 1])) <= 0) k--;
                h[k++] = a[i];
        }
        h.resize(k - 1);
        return h;
}

// Точка в произвольном многоугольнике: 0 -- снаружи, 1 -- на границе, 2 -- внутри. O(n)
template <typename T>
int pip(const ve<mpt<T>> &p, const mpt<T> &q) {
        int n = p.size(), in = 0;
        for (int i = 0; i < n; i++) {
                const mpt<T> &a = p[i], &b = p[(i + 1) % n];
                if (onSeg(q, a, b)) return 1;
                if ((sgn(a.y - q.y) > 0) != (sgn(b.y - q.y) > 0)) {
                        int s = sgn((b - a) % (q - a));
                        if (s == (b.y > a.y ? 1 : -1)) in ^= 1;
                }
        }
        return in ? 2 : 0;
}

// Точка в выпуклом ccw многоугольнике (с границей), O(log n)
template <typename T>
bool pinConvex(const ve<mpt<T>> &p, const mpt<T> &q) {
        int n = p.size();
        if (n == 1) return p[0] == q;
        if (n == 2) return onSeg(q, p[0], p[1]);
        if (sgn((p[1] - p[0]) % (q - p[0])) < 0 || sgn((p[n - 1] - p[0]) % (q - p[0])) > 0) return false;
        int l = 1, r = n - 1;
        while (r - l > 1) {
                int m = (l + r) / 2;
                if (sgn((p[m] - p[0]) % (q - p[0])) >= 0) l = m;
                else r = m;
        }
        return sgn((p[l + 1] - p[l]) % (q - p[l])) >= 0;
}

// Диаметр выпуклого многоугольника (вращающиеся калиперы), квадрат расстояния. h = hull(...)
template <typename T>
T diameter2(const ve<mpt<T>> &h) {
        int n = h.size();
        if (n == 1) return 0;
        if (n == 2) return (h[0] - h[1]).len2();
        T best = 0;
        for (int i = 0, j = 1; i < n; i++) {
                while (sgn((h[(i + 1) % n] - h[i]) % (h[(j + 1) % n] - h[j])) > 0) j = (j + 1) % n;
                best = max(best, max((h[i] - h[j]).len2(), (h[(i + 1) % n] - h[j]).len2()));
        }
        return best;
}

// Сумма Минковского выпуклых ccw многоугольников (hull-формат), O(n + m)
template <typename T>
ve<mpt<T>> minkowski(ve<mpt<T>> a, ve<mpt<T>> b) {
        auto fix = [](ve<mpt<T>> &p) {
                int k = 0;
                for (int i = 1; i < (int)p.size(); i++) {
                        if (sgn(p[i].y - p[k].y) < 0 || (sgn(p[i].y - p[k].y) == 0 && sgn(p[i].x - p[k].x) < 0)) k = i;
                }
                rotate(p.begin(), p.begin() + k, p.end());
                p.push_back(p[0]);
                p.push_back(p[1 % (p.size() - 1)]);
        };
        fix(a);
        fix(b);
        ve<mpt<T>> res;
        size_t i = 0, j = 0;
        while (i < a.size() - 2 || j < b.size() - 2) {
                res.push_back(a[i] + b[j]);
                int c = sgn((a[i + 1] - a[i]) % (b[j + 1] - b[j]));
                if (c >= 0 && i < a.size() - 2) i++;
                if (c <= 0 && j < b.size() - 2) j++;
        }
        return res;
}

// Ближайшая пара точек: min квадрат расстояния, n >= 2. O(n log n)
template <typename T>
T closestPair(ve<mpt<T>> p) {
        sort(all(p));
        int n = p.size();
        T best = numeric_limits<T>::max();
        ve<mpt<T>> tmp(n);
        auto byY = [](const mpt<T> &a, const mpt<T> &b) { return a.y < b.y; };
        function<void(int, int)> rec = [&](int l, int r) {
                if (r - l <= 3) {
                        for (int i = l; i < r; i++) for (int j = i + 1; j < r; j++) best = min(best, (p[i] - p[j]).len2());
                        sort(p.begin() + l, p.begin() + r, byY);
                        return;
                }
                int m = (l + r) / 2;
                T midx = p[m].x;
                rec(l, m);
                rec(m, r);
                merge(p.begin() + l, p.begin() + m, p.begin() + m, p.begin() + r, tmp.begin(), byY);
                copy(tmp.begin(), tmp.begin() + (r - l), p.begin() + l);
                int k = 0;
                for (int i = l; i < r; i++) {
                        if ((p[i].x - midx) * (p[i].x - midx) >= best) continue;
                        for (int j = k - 1; j >= 0 && (p[i].y - tmp[j].y) * (p[i].y - tmp[j].y) < best; j--) {
                                best = min(best, (p[i] - tmp[j]).len2());
                        }
                        tmp[k++] = p[i];
                }
        };
        rec(0, n);
        return best;
}

// Минимальная покрывающая окружность (Вельцль, ожидаемое O(n))
cr minCircle(ve<pt> p) {
        static mt19937 gen(12345);
        shuffle(all(p), gen);
        cr res(p[0], 0);
        auto out = [&](const pt &q) { return sgn(pt(res.c, q).len() - res.r) > 0; };
        for (size_t i = 1; i < p.size(); i++) {
                if (!out(p[i])) continue;
                res = cr(p[i], 0);
                for (size_t j = 0; j < i; j++) {
                        if (!out(p[j])) continue;
                        res = cr((p[i] + p[j]) / 2, pt(p[i], p[j]).len() / 2);
                        for (size_t k = 0; k < j; k++) {
                                if (out(p[k])) res = circum(p[i], p[j], p[k]);
                        }
                }
        }
        return res;
}

// Пересечение полуплоскостей. Полуплоскость -- слева от луча p + t * d.
struct HP {
        pt p, d;
        ld ang;
        HP(const pt &_p, const pt &_d) : p(_p), d(_d), ang(atan2l(_d.y, _d.x)) {}
        bool out(const pt &q) const { return sgn(d % (q - p)) < 0; }  // q строго снаружи
        bool operator<(const HP &o) const { return ang < o.ang; }
};

pt hpInter(const HP &a, const HP &b) {
        return a.p + a.d * (((b.p - a.p) % b.d) / (a.d % b.d));
}

// Многоугольник пересечения (ccw) или пусто. Добавьте большой bounding box, если область не ограничена.
ve<pt> hpi(ve<HP> H) {
        sort(all(H));
        deque<HP> dq;
        for (size_t i = 0; i < H.size(); i++) {
                while (dq.size() > 1 && H[i].out(hpInter(dq[dq.size() - 1], dq[dq.size() - 2]))) dq.pop_back();
                while (dq.size() > 1 && H[i].out(hpInter(dq[0], dq[1]))) dq.pop_front();
                if (!dq.empty() && sgn(H[i].d % dq.back().d) == 0) {
                        if (sgn(H[i].d * dq.back().d) < 0) return {};
                        if (H[i].out(dq.back().p)) dq.pop_back();
                        else continue;
                }
                dq.push_back(H[i]);
        }
        while (dq.size() > 2 && dq[0].out(hpInter(dq[dq.size() - 1], dq[dq.size() - 2]))) dq.pop_back();
        while (dq.size() > 2 && dq.back().out(hpInter(dq[0], dq[1]))) dq.pop_front();
        if (dq.size() < 3) return {};
        ve<pt> res;
        for (size_t i = 0; i < dq.size(); i++) res.push_back(hpInter(dq[i], dq[(i + 1) % dq.size()]));
        return res;
}

// Площадь пересечения круга (c, r) с простым многоугольником p (со знаком ориентации), O(n)
ld circlePoly(const ve<pt> &p, const pt &c, ld r) {
        auto arg = [](const pt &a, const pt &b) { return atan2l(a % b, a * b); };
        auto tri = [&](const pt &a, const pt &b) -> ld {
                ld r2 = r * r / 2;
                pt d = b - a;
                ld A = (d * a) / d.len2(), B = (a.len2() - r * r) / d.len2(), det = A * A - B;
                if (det <= 0) return arg(a, b) * r2;
                ld s = max<ld>(0, -A - sqrtl(det)), t = min<ld>(1, -A + sqrtl(det));
                if (t < 0 || 1 <= s) return arg(a, b) * r2;
                pt u = a + d * s, v = a + d * t;
                return arg(a, u) * r2 + (u % v) / 2 + arg(v, b) * r2;
        };
        ld sum = 0;
        for (size_t i = 0; i < p.size(); i++) sum += tri(p[i] - c, p[(i + 1) % p.size()] - c);
        return sum;
}
