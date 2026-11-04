#include "common.h"
#include "graphs/scc.cpp"
#include "graphs/bridges.cpp"

int comps(int n, const ve<pii> &es, int skipV, int skipE) {
        vi p(n); iota(all(p), 0);
        function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
        for (int i = 0; i < (int)es.size(); i++) { if (i == skipE || es[i].first == skipV || es[i].second == skipV) continue; p[f(es[i].first)] = f(es[i].second); }
        int c = 0; for (int v = 0; v < n; v++) if (v != skipV && f(v) == v) c++;
        return c;
}

signed main() {
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 8), m = rnd(0, 14);
                SCC s(n); ve<vb> r(n, vb(n));
                for (int i = 0; i < n; i++) r[i][i] = true;
                for (int i = 0; i < m; i++) { int a = rnd(0, n - 1), b = rnd(0, n - 1); s.addEdge(a, b); r[a][b] = true; }
                for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (r[i][k] && r[k][j]) r[i][j] = true;
                s.run();
                for (int a = 0; a < n; a++) for (int b = 0; b < n; b++) {
                        CHECK_EQ(s.comp[a] == s.comp[b], (bool)(r[a][b] && r[b][a]));
                        if (r[a][b] && !r[b][a]) CHECK(s.comp[a] > s.comp[b]);
                }
        }
        for (int it = 0; it < 6000; it++) {
                int n = rnd(1, 6), m = rnd(0, 12);
                TwoSat ts(n); ve<pii> cl;
                for (int i = 0; i < m; i++) { int a = rnd(0, 2 * n - 1), b = rnd(0, 2 * n - 1); ts.addClause(a, b); cl.push_back({a, b}); }
                bool ex = false;
                for (int mask = 0; mask < (1 << n) && !ex; mask++) {
                        auto lit = [&](int l) { bool v = mask >> (l / 2) & 1; return (l & 1) ? !v : v; };
                        bool ok = true; for (auto [a, b] : cl) if (!lit(a) && !lit(b)) ok = false;
                        ex |= ok;
                }
                bool got = ts.solve();
                CHECK_EQ(got, ex);
                if (got) for (auto [a, b] : cl) { auto lit = [&](int l) { bool v = ts.val[l / 2]; return (l & 1) ? !v : v; }; CHECK(lit(a) || lit(b)); }
        }
        for (int it = 0; it < 5000; it++) {
                int n = rnd(1, 9), m = rnd(0, 12);
                ve<pii> es; for (int i = 0; i < m; i++) { int a = rnd(0, n - 1), b = rnd(0, n - 1); if (a != b) es.push_back({a, b}); }
                Bridges br(n, es);
                int base = comps(n, es, -1, -1);
                for (int v = 0; v < n; v++) {
                        bool iso = true; for (auto [a, b] : es) if (a == v || b == v) iso = false;
                        // удаление v: компонент +- ; точка сочленения, если число компонент (без v) выросло
                        int after = comps(n, es, v, -1) ;
                        bool cut = after > base - (iso ? 1 : 0);
                        CHECK_EQ((bool)br.isCut[v], cut);
                        // число BCC, содержащих v = компоненты G - v, соседние с v
                        int cnt = 0; for (auto &b : br.bcc) if (find(all(b), v) != b.end()) cnt++;
                        int nb = 0;
                        { vi p(n); iota(all(p), 0); function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
                          for (auto [a, b] : es) if (a != v && b != v) p[f(a)] = f(b);
                          set<int> cs; for (auto [a, b] : es) { if (a == v && b != v) cs.insert(f(b)); if (b == v && a != v) cs.insert(f(a)); } nb = cs.size(); }
                        CHECK_EQ(cnt, iso ? 1 : nb);
                }
                for (int i = 0; i < (int)es.size(); i++) CHECK_EQ((bool)br.isBridge[i], comps(n, es, -1, i) > base);
        }
        cout << "ok\n";
}
