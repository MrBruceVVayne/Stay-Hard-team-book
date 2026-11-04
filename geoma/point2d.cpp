#pragma once
#include "tmp.cpp"

// База геометрии (стиль: mpt/mln/mcr + псевдонимы pt/ln/cr). Один код для ld и ll:
// sgn сравнивает с eps, для целых это точное сравнение. ll-точки -- для точных предикатов.
const ld eps = 1e-9;
const ld PI = acosl(-1);

template <typename T>
int sgn(T a) {
        return (a > eps) - (a < -eps);
}

bool lt(ld a, ld b) { return sgn(b - a) > 0; }
bool eq(ld a, ld b) { return sgn(a - b) == 0; }

template <typename T>
struct mpt {
        T x, y;
        mpt() : x(0), y(0) {}
        mpt(T _x, T _y) : x(_x), y(_y) {}
        mpt(const mpt &a, const mpt &b) : x(b.x - a.x), y(b.y - a.y) {}  // вектор a -> b

        mpt operator+(const mpt &a) const { return mpt(x + a.x, y + a.y); }
        mpt operator-(const mpt &a) const { return mpt(x - a.x, y - a.y); }
        mpt operator*(T k) const { return mpt(x * k, y * k); }
        mpt operator/(T k) const { return mpt(x / k, y / k); }
        T operator*(const mpt &a) const { return x * a.x + y * a.y; }  // скалярное
        T operator%(const mpt &a) const { return x * a.y - y * a.x; }  // векторное
        bool operator==(const mpt &a) const { return sgn(x - a.x) == 0 && sgn(y - a.y) == 0; }
        bool operator<(const mpt &a) const { return sgn(x - a.x) ? x < a.x : sgn(y - a.y) < 0; }

        T len2() const { return x * x + y * y; }
        ld len() const { return sqrtl(len2()); }
        mpt turn90() const { return mpt(-y, x); }
        mpt rot(ld a) const { return mpt(x * cosl(a) - y * sinl(a), x * sinl(a) + y * cosl(a)); }
        ld angle(const mpt &a) const { return atan2l(*this % a, *this * a); }  // угол от this к a, (-pi, pi]
        ld pol() const { return atan2l(y, x); }
};

using pt = mpt<ld>;
using ipt = mpt<ll>;

template <typename T>
istream& operator>>(istream &is, mpt<T> &a) {
        return is >> a.x >> a.y;
}

template <typename T>
ostream& operator<<(ostream &os, const mpt<T> &a) {
        return os << a.x << ' ' << a.y;
}

// Прямая a x + b y + c = 0. g(p) > 0 справа от направления p -> q (конструктор по 2 точкам).
template <typename T>
struct mln {
        T a, b, c;
        mln() : a(0), b(0), c(0) {}
        mln(T _a, T _b, T _c) : a(_a), b(_b), c(_c) {}
        mln(const mpt<T> &t, const mpt<T> &q) {
                mpt<T> p(t, q);
                a = p.y;
                b = -p.x;
                c = -a * q.x - b * q.y;
        }

        T g(const mpt<T> &p) const { return a * p.x + b * p.y + c; }
        mpt<T> nr() const { return mpt<T>(a, b); }   // нормаль
        mpt<T> pr() const { return mpt<T>(-b, a); }  // направление
        ld dist(const mpt<T> &p) const { return fabsl((ld)g(p)) / nr().len(); }
        mpt<T> proj(const mpt<T> &p) const {
                mpt<T> n = nr();
                return p - n * (g(p) / n.len2());
        }
};

using ln = mln<ld>;

// Пересечение непараллельных прямых
pt inter(const ln &l, const ln &m) {
        ld d = l.a * m.b - l.b * m.a;
        return pt((l.b * m.c - l.c * m.b) / d, (l.c * m.a - l.a * m.c) / d);
}

bool parallel(const ln &l, const ln &m) {
        return sgn(l.a * m.b - l.b * m.a) == 0;
}

// Окружность. fln/fcr возвращают 0, 1 или 2 точки; совпадающие окружности -> пустой вектор
// (проверять c == w.c && r == w.r снаружи).
template <typename T>
struct mcr {
        mpt<T> c;
        T r;
        mcr() : r(0) {}
        mcr(const mpt<T> &_c, T _r) : c(_c), r(_r) {}

        ve<pt> fln(const ln &l) const {
                ld d = l.dist(c);
                if (lt(r, d)) return {};
                pt p = l.proj(c);
                if (eq(d, r)) return {p};
                pt v = l.pr();
                v = v * (sqrtl(max<ld>(0, r * r - d * d)) / v.len());
                return {p + v, p - v};
        }

        ve<pt> fcr(const mcr &w) const {
                if (c == w.c) return {};
                ln l(2 * (c.x - w.c.x), 2 * (c.y - w.c.y),
                     r * r - w.r * w.r + w.c.len2() - c.len2());
                return fln(l);
        }

        ve<pt> tang(const pt &p) const {  // точки касания касательных из p
                pt v(c, p);
                ld d2 = v.len2();
                if (lt(d2, r * r)) return {};
                if (eq(d2, r * r)) return {p};
                return fcr(mcr<ld>(p, sqrtl(d2 - r * r)));
        }
};

using cr = mcr<ld>;

// Описанная окружность треугольника (не вырожденного)
cr circum(const pt &a, const pt &b, const pt &c) {
        pt ab(a, b), ac(a, c);
        ld d = 2 * (ab % ac);
        pt o(ac.y * ab.len2() - ab.y * ac.len2(), ab.x * ac.len2() - ac.x * ab.len2());
        o = o / d;
        return cr(a + o, o.len());
}

template <typename T>
bool onSeg(const mpt<T> &p, const mpt<T> &a, const mpt<T> &b) {
        return sgn((a - p) % (b - p)) == 0 && sgn((a - p) * (b - p)) <= 0;
}

// Пересекаются ли замкнутые отрезки ab и cd
template <typename T>
bool segInter(const mpt<T> &a, const mpt<T> &b, const mpt<T> &c, const mpt<T> &d) {
        int d1 = sgn((b - a) % (c - a)), d2 = sgn((b - a) % (d - a));
        int d3 = sgn((d - c) % (a - c)), d4 = sgn((d - c) % (b - c));
        if (d1 * d2 < 0 && d3 * d4 < 0) return true;
        return onSeg(c, a, b) || onSeg(d, a, b) || onSeg(a, c, d) || onSeg(b, c, d);
}

ld segDist(const pt &p, const pt &a, const pt &b) {
        pt v(a, b);
        if (sgn(v.len2()) == 0) return pt(p, a).len();
        ld t = ((p - a) * v) / v.len2();
        t = max<ld>(0, min<ld>(1, t));
        return pt(p, a + v * t).len();
}

// Полярная сортировка: полуплоскость, затем векторное произведение
template <typename T>
int half(const mpt<T> &p) {
        return (sgn(p.y) > 0 || (sgn(p.y) == 0 && sgn(p.x) >= 0)) ? 0 : 1;
}

template <typename T>
bool angLess(const mpt<T> &l, const mpt<T> &r) {
        if (half(l) != half(r)) return half(l) < half(r);
        return sgn(l % r) > 0;
}
