#include "common.h"
#include "graphs/tree.cpp"
#include "graphs/paths.cpp"

signed main() {
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 30);
                HLD h(n); Centroid c(n); ve<vi> g(n); vi par(n, -1);
                for (int v = 1; v < n; v++) { int p = rnd(0, 1) ? rnd(0, v - 1) : max(0, v - 1 - (int)rnd(0, 2)); h.addEdge(v, p); c.addEdge(v, p); g[v].push_back(p); g[p].push_back(v); }
                h.build(); c.build();
                vi dep(n, 0);
                { function<void(int, int)> d = [&](int v, int p) { par[v] = p; for (int u : g[v]) if (u != p) dep[u] = dep[v] + 1, d(u, v); }; d(0, -1); }
                vll val(n); for (auto &x : val) x = rnd(-9, 9);
                vll arr(n); for (int v = 0; v < n; v++) arr[h.pos[v]] = val[v];
                auto dist = [&](int a, int b) { int d = 0; while (a != b) { if (dep[a] < dep[b]) swap(a, b); a = par[a]; d++; } return d; };
                for (int q = 0; q < 30; q++) {
                        int a = rnd(0, n - 1), b = rnd(0, n - 1);
                        int x = a, y = b; while (x != y) { if (dep[x] < dep[y]) swap(x, y); x = par[x]; }
                        CHECK_EQ(h.lca(a, b), x);
                        ll s = 0, sp = 0; x = a; y = b;
                        while (x != y) { if (dep[x] < dep[y]) swap(x, y); s += val[x]; x = par[x]; } s += val[x];
                        for (auto [l, r] : h.path(a, b)) for (int i = l; i <= r; i++) sp += arr[i];
                        CHECK_EQ(sp, s);
                        // центроиды: lca в дереве центроидов лежит на пути
                        int u = a, v = b; while (u != v) { if (c.clvl[u] < c.clvl[v]) swap(u, v); u = c.cpar[u]; }
                        CHECK_EQ(dist(a, u) + dist(u, b), dist(a, b));
                }
                int mx = 0; for (int v = 0; v < n; v++) mx = max(mx, c.clvl[v]);
                CHECK(mx <= 5);
                for (int v = 0; v < n; v++) { int cnt = 0; for (int i = 0; i < n; i++) if (i >= h.pos[v] && i < h.pos[v] + h.sz[v]) cnt++; CHECK_EQ(cnt, h.sz[v]); }
                for (int v = 0; v < n; v++) for (int u = 0; u < n; u++) if (h.lca(v, u) == v) CHECK(h.pos[u] >= h.pos[v] && h.pos[u] < h.pos[v] + h.sz[v]);
        }
        // кратчайшие пути
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 7), m = rnd(0, 14);
                ve<array<ll, 3>> es; ve<vll> d(n, vll(n, INFLL)); ve<ve<pair<int, ll>>> g(n);
                for (int i = 0; i < n; i++) d[i][i] = 0;
                bool neg = it % 2;
                for (int i = 0; i < m; i++) { int a = rnd(0, n - 1), b = rnd(0, n - 1); ll w = rnd(neg ? -3 : 0, 9); es.push_back({a, b, w}); g[a].push_back({b, w}); d[a][b] = min(d[a][b], w); }
                if (!neg) { auto dj = dijkstra(g, 0); auto f = d; floyd(f); for (int v = 0; v < n; v++) CHECK_EQ(dj[v], f[0][v]); }
                auto f = d; floyd(f);
                bool hasNeg = false; for (int v = 0; v < n; v++) if (f[0][v] < INFLL && f[v][v] < 0) hasNeg = true;
                vll bd; bool ok = bellmanFord(n, es, 0, bd);
                CHECK_EQ(ok, !hasNeg);
                if (ok) for (int v = 0; v < n; v++) CHECK_EQ(bd[v], f[0][v]);
        }
        // Эйлеров путь
        for (int it = 0; it < 6000; it++) {
                int n = rnd(1, 5), m = rnd(0, 7); bool dir = it % 2;
                ve<pii> es; for (int i = 0; i < m; i++) es.push_back({rnd(0, n - 1), rnd(0, n - 1)});
                vi res = eulerPath(n, es, dir);
                // брутфорс: поиск пути по всем перестановкам рёбер (DFS)
                bool ex = (m == 0); vb used(m);
                function<void(int, int)> go = [&](int v, int cnt) {
                        if (ex) return; if (cnt == m) { ex = true; return; }
                        for (int i = 0; i < m; i++) { if (used[i]) continue; int a = es[i].first, b = es[i].second;
                                if (a == v) { used[i] = true; go(b, cnt + 1); used[i] = false; }
                                if (!dir && b == v && a != b) { used[i] = true; go(a, cnt + 1); used[i] = false; } }
                };
                for (int s = 0; s < n && !ex; s++) go(s, 0);
                CHECK_EQ(!res.empty(), ex);
                if (!res.empty() && m > 0) {
                        CHECK_EQ((int)res.size(), m + 1);
                        multiset<pii> have; for (auto [a, b] : es) have.insert(dir || a < b ? pii(a, b) : pii(b, a));
                        for (int i = 0; i + 1 < (int)res.size(); i++) { int a = res[i], b = res[i + 1]; auto key = dir || a < b ? pii(a, b) : pii(b, a); auto itx = have.find(key); CHECK(itx != have.end()); have.erase(itx); }
                        CHECK(have.empty());
                }
        }
        // направленное остовное дерево
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 6), m = rnd(0, 12);
                ve<array<ll, 3>> es; for (int i = 0; i < m; i++) es.push_back({rnd(0, n - 1), rnd(0, n - 1), rnd(0, 9)});
                ll best = INFLL;
                // для каждой вершины, кроме корня 0, выбрать входящее ребро
                ve<vi> in(n); for (int i = 0; i < m; i++) if (es[i][0] != es[i][1] && es[i][1] != 0) in[es[i][1]].push_back(i);
                vi ch(n, -1);
                function<void(int)> rec = [&](int v) {
                        if (v == n) { for (int u = 1; u < n; u++) { int x = u, st = 0; while (x != 0 && st <= n) x = es[ch[x]][0], st++; if (x != 0) return; } ll c = 0; for (int u = 1; u < n; u++) c += es[ch[u]][2]; best = min(best, c); return; }
                        if (v == 0) { rec(1); return; }
                        for (int id : in[v]) ch[v] = id, rec(v + 1);
                };
                rec(0);
                CHECK_EQ(dirMST(n, 0, es), best == INFLL ? -1 : best);
        }
        cout << "ok\n";
}
