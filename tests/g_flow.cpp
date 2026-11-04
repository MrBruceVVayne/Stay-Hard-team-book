#include "common.h"
#include "graphs/dinic.cpp"
#include "graphs/mincost.cpp"

signed main() {
        for (int it = 0; it < 3000; it++) {
                int n = rnd(2, 7), m = rnd(1, 14);
                ve<array<ll, 3>> es;
                for (int i = 0; i < m; i++) es.push_back({rnd(0, n - 1), rnd(0, n - 1), rnd(0, 6)});
                // макс-флоу: Эдмондс-Карп на матрице
                ve<vll> cap(n, vll(n));
                Dinic d(n);
                for (auto [a, b, c] : es) { cap[a][b] += c; d.addEdge(a, b, c); }
                int s = 0, t = n - 1; ll fl = 0;
                while (true) {
                        vi par(n, -1); par[s] = s; queue<int> q; q.push(s);
                        while (!q.empty()) { int v = q.front(); q.pop(); for (int u = 0; u < n; u++) if (par[u] == -1 && cap[v][u] > 0) par[u] = v, q.push(u); }
                        if (par[t] == -1) break;
                        ll f = INFLL; for (int v = t; v != s; v = par[v]) f = min(f, cap[par[v]][v]);
                        for (int v = t; v != s; v = par[v]) cap[par[v]][v] -= f, cap[v][par[v]] += f;
                        fl += f;
                }
                CHECK_EQ(d.maxflow(s, t), fl);
                // разрез = поток
                ll cut = 0; for (auto [a, b, c] : es) if (d.inS(a) && !d.inS(b)) cut += c;
                CHECK(d.inS(s) && !d.inS(t));
                CHECK_EQ(cut, fl);
        }
        // Гомори-Ху и Штор-Вагнер
        for (int it = 0; it < 1500; it++) {
                int n = rnd(2, 7), m = rnd(1, 12);
                ve<array<ll, 3>> es; ve<vll> w(n, vll(n));
                for (int i = 0; i < m; i++) { int a = rnd(0, n - 1), b = rnd(0, n - 1); if (a == b) continue; ll c = rnd(1, 5); es.push_back({a, b, c}); w[a][b] += c; w[b][a] += c; }
                auto [par, wt] = gomoryHu(n, es);
                for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++) {
                        Dinic d(n); for (auto [a, b, c] : es) d.addEdge(a, b, c, c);
                        ll f = d.maxflow(u, v);
                        // путь в дереве
                        ve<int> pu, pv; for (int x = u; ; x = par[x]) { pu.push_back(x); if (x == 0) break; } for (int x = v; ; x = par[x]) { pv.push_back(x); if (x == 0) break; }
                        ll mn = INFLL; set<int> su(all(pu));
                        int l = -1; for (int x : pv) if (su.count(x)) { l = x; break; }
                        for (int x = u; x != l; x = par[x]) mn = min(mn, wt[x]);
                        for (int x = v; x != l; x = par[x]) mn = min(mn, wt[x]);
                        CHECK_EQ(mn, f);
                }
                ll best = INFLL;
                for (int mask = 1; mask < (1 << n) - 1; mask++) { ll c = 0; for (int a = 0; a < n; a++) for (int b = 0; b < n; b++) if ((mask >> a & 1) && !(mask >> b & 1)) c += w[a][b]; best = min(best, c); }
                CHECK_EQ(stoerWagner(w), best);
        }
        // mincost maxflow: перебор всех потоков
        for (int it = 0; it < 1500; it++) {
                int n = rnd(2, 5), m = rnd(1, 7);
                ve<array<ll, 4>> es; MCMF mc(n);
                for (int i = 0; i < m; i++) { int a = rnd(0, n - 1), b = rnd(0, n - 1); if (a == b) continue; es.push_back({a, b, rnd(1, 2), rnd(0, 5)}); mc.addEdge(a, b, es.back()[2], es.back()[3]); }
                auto [fl, cost] = mc.run(0, n - 1);
                int k = es.size(); ll bf = 0, bc = 0; vll f(k);
                function<void(int)> rec = [&](int i) {
                        if (i == k) {
                                vll bal(n); ll c = 0;
                                for (int j = 0; j < k; j++) { bal[es[j][0]] -= f[j]; bal[es[j][1]] += f[j]; c += f[j] * es[j][3]; }
                                for (int v = 1; v < n - 1; v++) if (bal[v]) return;
                                ll val = bal[n - 1];
                                if (val > bf || (val == bf && c < bc)) bf = val, bc = c;
                                return;
                        }
                        for (ll x = 0; x <= es[i][2]; x++) f[i] = x, rec(i + 1);
                };
                rec(0);
                CHECK_EQ(fl, bf); CHECK_EQ(cost, bc);
        }
        cout << "ok\n";
}
