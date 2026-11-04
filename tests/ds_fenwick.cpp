#include "common.h"
#include "dataStructures/fenwick.cpp"
#include "dataStructures/sptable.cpp"
#include "dataStructures/dst.cpp"

signed main() {
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 40);
                ve<ll> a(n);
                for (auto &x : a) x = rnd(0, 20);
                FenwickTree<ll> f(a), g(n);
                for (int i = 0; i < n; i++) g.update(i, a[i]);
                for (int q = 0; q < 30; q++) {
                        int t = rnd(0, 2);
                        if (t == 0) {
                                int i = rnd(0, n - 1); ll x = rnd(0, 5);
                                a[i] += x; f.update(i, x); g.update(i, x);
                        } else if (t == 1) {
                                int l = rnd(0, n), r = rnd(l, n);
                                ll s = 0;
                                for (int i = l; i < r; i++) s += a[i];
                                CHECK_EQ(f.get(l, r), s);
                                CHECK_EQ(g.get(l, r), s);
                        } else {
                                ll pos = rnd(0, 200);
                                int k = 0; ll s = 0;
                                while (k < n && s + a[k] <= pos) s += a[k++];
                                CHECK_EQ(f.lower_bound(pos), k);
                        }
                }
        }
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 70);
                vi a(n);
                for (auto &x : a) x = rnd(-50, 50);
                SparseTable sp(a);
                DST d(a);
                for (int l = 0; l < n; l++) for (int r = l + 1; r <= n; r++) {
                        int m = INF;
                        for (int i = l; i < r; i++) m = min(m, a[i]);
                        CHECK_EQ(sp.get(l, r), m);
                        CHECK_EQ(d.get(l, r).x, m);
                }
        }
        cout << "ok\n";
}
