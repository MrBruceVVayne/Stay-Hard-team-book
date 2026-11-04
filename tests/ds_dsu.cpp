#include "common.h"
#include "dataStructures/dsu.cpp"

signed main() {
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 15);
                DSU d(n);
                ve<vi> comp(n);  // наивно: метка компоненты
                vi lab(n);
                iota(all(lab), 0);
                ve<pr<int, vi>> saved;
                for (int q = 0; q < 40; q++) {
                        int t = rnd(0, 3);
                        if (t <= 1) {
                                int a = rnd(0, n - 1), b = rnd(0, n - 1);
                                bool merged = d.unite(a, b);
                                CHECK_EQ(merged, lab[a] != lab[b]);
                                int la = lab[a], lb = lab[b];
                                for (auto &x : lab) if (x == lb) x = la;
                        } else if (t == 2) {
                                saved.push_back({d.snapshot(), lab});
                        } else if (!saved.empty()) {
                                d.rollback(saved.back().first);
                                lab = saved.back().second;
                                saved.pop_back();
                        }
                        for (int a = 0; a < n; a++) for (int b = 0; b < n; b++)
                                CHECK_EQ(d.find(a) == d.find(b), lab[a] == lab[b]);
                }
        }
        cout << "ok\n";
}
