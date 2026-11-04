#include "tmp.cpp"

// Дерево Ли Чао: максимум прямых в целых точках x in [0, n). Прямые можно добавлять
// в любом порядке. Модифицируется до отрезков (add на [lo, hi)) обходом как в ДО.
struct LiChao {
        struct Line {
                ll k = 0, b = -INFLL;
                ll operator()(ll x) const { return k * x + b; }
        };

        int n;
        ve<Line> t;

        LiChao() {}
        LiChao(int _n) : n(_n), t(4 * _n + 4) {}

        void add(Line ln, int v, int l, int r) {  // [l, r)
                int mid = (l + r) / 2;
                bool lef = ln(l) > t[v](l), md = ln(mid) > t[v](mid);
                if (md) swap(t[v], ln);
                if (r - l == 1) return;
                if (lef != md) add(ln, 2 * v, l, mid);
                else add(ln, 2 * v + 1, mid, r);
        }

        void add(ll k, ll b) {
                Line ln;
                ln.k = k;
                ln.b = b;
                add(ln, 1, 0, n);
        }

        ll get(int x) {
                ll res = -INFLL;
                int v = 1, l = 0, r = n;
                while (true) {
                        res = max(res, t[v](x));
                        if (r - l == 1) break;
                        int mid = (l + r) / 2;
                        if (x < mid) v = 2 * v, r = mid;
                        else v = 2 * v + 1, l = mid;
                }
                return res;
        }
};
