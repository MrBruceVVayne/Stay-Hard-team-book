#include "common.h"
#include "dataStructures/linkCutTree.cpp"

// брутфорс: список смежности, путь поиском в глубину
int n;
ve<set<int>> g;
bool path(int v, int p, int to, ve<int> &out) {
        out.push_back(v);
        if (v == to) return true;
        for (int u : g[v]) if (u != p && path(u, v, to, out)) return true;
        out.pop_back();
        return false;
}

signed main() {
        for (int it = 0; it < 3000; it++) {
                n = rnd(2, 12);
                LinkCut lct(n);
                g.assign(n + 1, {});
                ve<ll> val(n + 1, 0);
                for (int q = 0; q < 60; q++) {
                        int op = rnd(0, 4), a = rnd(1, n), b = rnd(1, n);
                        ve<int> pth;
                        bool conn = path(a, 0, b, pth);
                        if (op == 0) {
                                bool ok = lct.link(a, b);
                                CHECK_EQ(ok, !conn);
                                if (ok) { g[a].insert(b); g[b].insert(a); }
                        } else if (op == 1) {
                                bool has = g[a].count(b);
                                bool ok = lct.cut(a, b);
                                CHECK_EQ(ok, has);
                                if (ok) { g[a].erase(b); g[b].erase(a); }
                        } else if (op == 2) {
                                CHECK_EQ(lct.connected(a, b), conn);
                        } else if (op == 3) {
                                ll v = rnd(-9, 9);
                                lct.set(a, v); val[a] = v;
                        } else if (conn) {
                                ll s = 0;
                                for (int v : pth) s += val[v];
                                CHECK_EQ(lct.pathSum(a, b), s);
                        }
                }
        }
        cout << "ok\n";
}
