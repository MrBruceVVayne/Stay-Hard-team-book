#include "common.h"
#include "dataStructures/segTree.cpp"
#include "dataStructures/segTreeFast.cpp"

signed main() {
        for (int it = 0; it < 2000; it++) {
                int n = rnd(1, 30);
                ve<ll> a(n);
                for (auto &x : a) x = rnd(-20, 20);
                SegTree st(a);
                for (int q = 0; q < 40; q++) {
                        int t = rnd(0, 3);
                        int l = rnd(0, n - 1), r = rnd(l + 1, n);
                        if (t == 0) {
                                ll x = rnd(-10, 10);
                                st.update(l, r, x);
                                for (int i = l; i < r; i++) a[i] += x;
                        } else if (t == 1) {
                                ll s = 0;
                                for (int i = l; i < r; i++) s += a[i];
                                CHECK_EQ(st.getSum(l, r), s);
                        } else if (t == 2) {
                                ll m = -INFLL;
                                for (int i = l; i < r; i++) m = max(m, a[i]);
                                CHECK_EQ(st.getMax(l, r), m);
                        } else {
                                ll x = rnd(-30, 30);
                                int ans = -1;
                                for (int i = l; i < n; i++) if (a[i] >= x) { ans = i; break; }
                                CHECK_EQ(st.firstGe(l, x), ans);
                        }
                }
        }
        // некоммутативная: композиция a*x+b
        for (int it = 0; it < 2000; it++) {
                int n = rnd(1, 30);
                using N = SegTreeFast::Node;
                ve<N> a(n);
                for (auto &x : a) x = N(rnd(0, MOD - 1), rnd(0, MOD - 1));
                SegTreeFast st(a);
                for (int q = 0; q < 30; q++) {
                        if (rnd(0, 1)) {
                                int i = rnd(0, n - 1);
                                a[i] = N(rnd(0, MOD - 1), rnd(0, MOD - 1));
                                st.update(i, a[i]);
                        } else {
                                int l = rnd(0, n - 1), r = rnd(l + 1, n);
                                ll x = rnd(0, MOD - 1), y = x;
                                for (int i = l; i < r; i++) y = (a[i].a * y + a[i].b) % MOD;
                                auto res = st.get(l, r);
                                CHECK_EQ((res.a * x + res.b) % MOD, y);
                        }
                }
        }
        cout << "ok\n";
}
