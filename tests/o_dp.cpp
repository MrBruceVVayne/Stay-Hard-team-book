#include "common.h"
#include "opts/dpopt.cpp"
#include "math/simplex.cpp"
#include "math/gauss.cpp"

signed main() {
        for (int it = 0; it < 3000; it++) {
                int n = rnd(2, 14);
                vll a(n), pre(n + 1);
                for (auto &x : a) x = rnd(0, 9);
                for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];
                // D&C: dp[j] = min dp[k] + (pre[j] - pre[k])^2 + C, слои
                auto cost = [&](int k, int j) { return (pre[j] - pre[k]) * (pre[j] - pre[k]); };
                vll prev(n + 1, INFLL); prev[0] = 0; vll bf = prev;
                for (int layer = 0; layer < 3; layer++) {
                        prev = dcLayer(prev, cost);
                        vll nx(n + 1, INFLL);
                        for (int j = 0; j <= n; j++) for (int k = 0; k < j; k++) if (bf[k] < INFLL) nx[j] = min(nx[j], bf[k] + cost(k, j));
                        bf = nx;
                        CHECK_EQ(prev, bf);
                }
                // Кнут: оптимальное слияние, cost = сумма на [l, r]
                auto c2 = [&](int l, int r) { return pre[r + 1] - pre[l]; };
                ve<vll> dp(n, vll(n, 0));
                for (int len = 2; len <= n; len++) for (int l = 0; l + len <= n; l++) { int r = l + len - 1; dp[l][r] = INFLL; for (int k = l; k < r; k++) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + c2(l, r)); }
                CHECK_EQ(knuth(n, c2), dp[0][n - 1]);
                // LIS
                vi b(n); for (auto &x : b) x = rnd(0, 6);
                vi f(n, 1); int best = n ? 1 : 0;
                for (int i = 0; i < n; i++) for (int j = 0; j < i; j++) if (b[j] < b[i]) f[i] = max(f[i], f[j] + 1), best = max(best, f[i]);
                vi l = lis(b); CHECK_EQ((int)l.size(), best);
                for (size_t i = 0; i + 1 < l.size(); i++) CHECK(l[i] < l[i + 1] && b[l[i]] < b[l[i + 1]]);
        }
        // симплекс против перебора вершин
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 3), m = rnd(1, 4);
                ve<ve<ld>> A(m, ve<ld>(n)); ve<ld> b(m), c(n);
                for (auto &r : A) for (auto &x : r) x = rnd(-3, 3);
                for (auto &x : b) x = rnd(-4, 8);
                for (auto &x : c) x = rnd(-3, 3);
                // ограничение сверху, чтобы был ограничен
                A.push_back(ve<ld>(n, 1)); b.push_back(10);
                int M = A.size();
                // все ограничения: A x <= b, -x_i <= 0
                ve<ve<ld>> rows; ve<ld> rhs;
                for (int i = 0; i < M; i++) rows.push_back(A[i]), rhs.push_back(b[i]);
                for (int i = 0; i < n; i++) { ve<ld> r(n, 0); r[i] = -1; rows.push_back(r); rhs.push_back(0); }
                int K = rows.size(); ld best = -1e18; bool feas = false;
                for (int mask = 0; mask < (1 << K); mask++) {
                        if (__builtin_popcount(mask) != n) continue;
                        ve<ve<ld>> sys; for (int i = 0; i < K; i++) if (mask >> i & 1) { auto r = rows[i]; r.push_back(rhs[i]); sys.push_back(r); }
                        ve<ld> x; int rk = gaussD(sys, x);
                        if (rk != n) continue;
                        bool ok = true; for (int i = 0; i < K; i++) { ld s = 0; for (int j = 0; j < n; j++) s += rows[i][j] * x[j]; if (s > rhs[i] + 1e-7) ok = false; }
                        if (!ok) continue;
                        feas = true; ld v = 0; for (int j = 0; j < n; j++) v += c[j] * x[j]; best = max(best, v);
                }
                LPSolver lp(A, b, c); ve<ld> x; ld res = lp.solve(x);
                if (!feas) CHECK(res < -1e17);
                else { CHECK(fabsl(res - best) < 1e-6); ld v = 0; for (int j = 0; j < n; j++) { v += c[j] * x[j]; CHECK(x[j] > -1e-7); } CHECK(fabsl(v - best) < 1e-6); }
        }
        cout << "ok\n";
}
