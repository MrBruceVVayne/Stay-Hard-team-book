#include "common.h"
#include "dataStructures/lineContainer.cpp"
#include "dataStructures/liChao.cpp"

signed main() {
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 40);
                LineContainer lc;
                LiChao li(n);
                ve<pll> ls;
                for (int q = 0; q < 40; q++) {
                        if (rnd(0, 1) || ls.empty()) {
                                ll k = rnd(-20, 20), m = rnd(-500, 500);
                                lc.add(k, m); li.add(k, m); ls.push_back({k, m});
                        } else {
                                ll x = rnd(0, n - 1), best = -INFLL;
                                for (auto [k, m] : ls) best = max(best, k * x + m);
                                CHECK_EQ(lc.query(x), best);
                                CHECK_EQ(li.get(x), best);
                        }
                }
        }
        cout << "ok\n";
}
