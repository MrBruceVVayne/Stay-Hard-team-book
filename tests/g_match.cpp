#include "common.h"
#include "graphs/matching.cpp"

signed main() {
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 6), m = rnd(1, 6), e = rnd(0, 14);
                HopcroftKarp hk(n, m); ve<pii> es; vi adj(n);
                for (int i = 0; i < e; i++) { int a = rnd(0, n - 1), b = rnd(0, m - 1); hk.addEdge(a, b); es.push_back({a, b}); adj[a] |= 1 << b; }
                // брутфорс: макс. паросочетание перебором подмножеств левых + мин. покрытие
                function<int(int, int)> go = [&](int v, int used) { if (v == n) return 0; int r = go(v + 1, used); for (int u = 0; u < m; u++) if ((adj[v] >> u & 1) && !(used >> u & 1)) r = max(r, 1 + go(v + 1, used | 1 << u)); return r; };
                int mm = go(0, 0);
                CHECK_EQ(hk.solve(), mm);
                auto [L, R] = hk.cover();
                CHECK_EQ((int)(L.size() + R.size()), mm);
                vb cl(n), cr(m); for (int v : L) cl[v] = true; for (int v : R) cr[v] = true;
                for (auto [a, b] : es) CHECK(cl[a] || cr[b]);
                for (int v = 0; v < n; v++) if (hk.ml[v] != -1) CHECK_EQ(hk.mr[hk.ml[v]], v);
        }
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 10), e = rnd(0, 20);
                Blossom b(n); ve<int> adj(n);
                for (int i = 0; i < e; i++) { int x = rnd(0, n - 1), y = rnd(0, n - 1); if (x == y) continue; b.addEdge(x, y); adj[x] |= 1 << y; adj[y] |= 1 << x; }
                vi dp(1 << n, 0);
                for (int mask = 1; mask < (1 << n); mask++) {
                        int v = __builtin_ctz(mask), rest = mask ^ (1 << v);
                        dp[mask] = dp[rest];
                        for (int u = 0; u < n; u++) if ((rest >> u & 1) && (adj[v] >> u & 1)) dp[mask] = max(dp[mask], 1 + dp[rest ^ (1 << u)]);
                }
                CHECK_EQ(b.solve(), dp[(1 << n) - 1]);
                for (int v = 0; v < n; v++) if (b.match[v] != -1) { CHECK_EQ(b.match[b.match[v]], v); CHECK(adj[v] >> b.match[v] & 1); }
        }
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 6), m = rnd(n, 7);
                ve<vll> a(n + 1, vll(m + 1));
                for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) a[i][j] = rnd(-9, 9);
                vi pm(m); iota(all(pm), 1); ll best = INFLL;
                do { ll c = 0; for (int i = 0; i < n; i++) c += a[i + 1][pm[i]]; best = min(best, c); } while (next_permutation(all(pm)));
                auto [cost, ans] = hungarian(a, n, m);
                CHECK_EQ(cost, best);
                ll c2 = 0; set<int> cols; for (int i = 1; i <= n; i++) { c2 += a[i][ans[i]]; cols.insert(ans[i]); }
                CHECK_EQ(c2, best); CHECK_EQ((int)cols.size(), n);
        }
        cout << "ok\n";
}
