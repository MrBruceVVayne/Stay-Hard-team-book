#include "common.h"
#include "dataStructures/treap.cpp"
#include "dataStructures/persTreap.cpp"

signed main() {
        for (int it = 0; it < 1500; it++) {
                Treap t;
                ve<ll> a;
                for (int q = 0; q < 80; q++) {
                        int op = rnd(0, 4), n = a.size();
                        if (op == 0 || n == 0) {
                                int pos = rnd(0, n); ll x = rnd(-9, 9);
                                t.insert(pos, x); a.insert(a.begin() + pos, x);
                        } else if (op == 1) {
                                int pos = rnd(0, n - 1);
                                t.erase(pos); a.erase(a.begin() + pos);
                        } else if (op == 2) {
                                int l = rnd(0, n - 1), r = rnd(l + 1, n);
                                t.reverse(l, r); reverse(a.begin() + l, a.begin() + r);
                        } else if (op == 3) {
                                int l = rnd(0, n), r = rnd(l, n);
                                ll s = 0;
                                for (int i = l; i < r; i++) s += a[i];
                                CHECK_EQ(t.sum(l, r), s);
                        } else {
                                int pos = rnd(0, n - 1);
                                CHECK_EQ(t.get(pos), a[pos]);
                        }
                        CHECK_EQ(t.size(), (int)a.size());
                }
                ve<ll> out;
                t.dump(t.root, out);
                CHECK_EQ(out, a);
        }
        for (int it = 0; it < 1500; it++) {
                PersTreap t;
                ve<ve<ll>> vers(1);
                ve<int> roots(1, 0);
                for (int q = 0; q < 60; q++) {
                        int base = rnd(0, (int)roots.size() - 1);
                        ve<ll> a = vers[base];
                        int root = roots[base], n = a.size();
                        int op = rnd(0, 2);
                        if (op == 0 || n == 0) {
                                int pos = rnd(0, n); ll x = rnd(-9, 9);
                                root = t.insert(root, pos, x); a.insert(a.begin() + pos, x);
                        } else if (op == 1) {
                                int pos = rnd(0, n - 1);
                                root = t.erase(root, pos); a.erase(a.begin() + pos);
                        } else {
                                int l = rnd(0, n), r = rnd(l, n);
                                ll s = 0;
                                for (int i = l; i < r; i++) s += a[i];
                                CHECK_EQ(t.sum(root, l, r), s);
                        }
                        CHECK_EQ(t.size(root), (int)a.size());
                        vers.push_back(a); roots.push_back(root);
                }
                for (size_t v = 0; v < roots.size(); v++)  // старые версии не испорчены
                        CHECK_EQ(t.sum(roots[v], 0, t.size(roots[v])), accumulate(all(vers[v]), 0LL));
        }
        cout << "ok\n";
}
