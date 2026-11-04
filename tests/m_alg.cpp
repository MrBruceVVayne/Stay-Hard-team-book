#include "common.h"
#include "opts/ntt.cpp"
#include "opts/fft.cpp"
#include "opts/fwht.cpp"
#include "math/gauss.cpp"
#include "math/linrec.cpp"
#include "math/combi.cpp"

signed main() {
        // NTT / FFT / произвольный модуль
        for (int it = 0; it < 400; it++) {
                int n = rnd(1, 40), m = rnd(1, 40), mod = (rnd(0, 1) ? 1000000007 : (int)rnd(2, 2000000000));
                ve<int> a(n), b(m);
                for (auto &x : a) x = rnd(0, 997);
                for (auto &x : b) x = rnd(0, 997);
                ve<ll> A(all(a)), B(all(b)), exp(n + m - 1);
                for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) exp[i + j] += (ll)a[i] * b[j];
                auto r = conv(a, b);
                auto rf = convFFT(A, B);
                for (int i = 0; i < n + m - 1; i++) { CHECK_EQ((ll)r[i], exp[i] % 998244353); CHECK_EQ(rf[i], exp[i]); }
                for (auto &x : a) x = rnd(0, mod - 1);
                for (auto &x : b) x = rnd(0, mod - 1);
                ve<ll> e2(n + m - 1);
                for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) e2[i + j] = (e2[i + j] + (__int128)a[i] * b[j]) % mod;
                auto rm = convMod(a, b, mod);
                for (int i = 0; i < n + m - 1; i++) CHECK_EQ((ll)rm[i], e2[i]);
        }
        // FWHT / SOS
        for (int it = 0; it < 500; it++) {
                int k = rnd(0, 6), n = 1 << k;
                vll a(n), b(n);
                for (auto &x : a) x = rnd(-5, 5);
                for (auto &x : b) x = rnd(-5, 5);
                for (int type = 0; type < 3; type++) {
                        vll e(n);
                        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
                                int t = type == 0 ? (i ^ j) : type == 1 ? (i & j) : (i | j);
                                e[t] += a[i] * b[j];
                        }
                        CHECK_EQ(xorConv(a, b, type), e);
                }
                vll f = a;
                sosSum(f, k);
                for (int m = 0; m < n; m++) { ll s = 0; for (int s2 = 0; s2 < n; s2++) if ((s2 & m) == s2) s += a[s2]; CHECK_EQ(f[m], s); }
                sosInv(f, k);
                CHECK_EQ(f, a);
        }
        // Гаусс
        for (int it = 0; it < 3000; it++) {
                int n = rnd(1, 6);
                ll p = (it % 2) ? 3 : (it % 4 == 0 ? 7 : MOD);
                ve<vll> a(n, vll(n));
                for (auto &r : a) for (auto &x : r) x = rnd(0, min<ll>(p - 1, 6));
                // определитель перестановками
                vi pm(n); iota(all(pm), 0); ll det = 0;
                do {
                        ll t = 1; int inv_ = 0;
                        for (int i = 0; i < n; i++) { t = t * a[i][pm[i]] % p; for (int j = i + 1; j < n; j++) inv_ += pm[i] > pm[j]; }
                        det = ((det + (inv_ & 1 ? p - t : t)) % p + p) % p;
                } while (next_permutation(all(pm)));
                CHECK_EQ(detMod(a, p), det % p);
                // система (может быть несовместна): перебор для p = 3, 7
                int m = rnd(1, 4);
                ve<vll> s(n, vll(m + 1));
                for (auto &r : s) for (auto &x : r) x = rnd(0, min<ll>(p - 1, 6));
                vll x; int rk = gaussMod(s, x, p);
                if (p < 10) {
                        bool ex = false; vll v(m, 0);
                        function<void(int)> rec = [&](int i) {
                                if (ex) return;
                                if (i == m) { bool ok = true; for (auto &r : s) { ll t = 0; for (int j = 0; j < m; j++) t += r[j] * v[j]; if ((t - r[m]) % p) ok = false; } ex |= ok; return; }
                                for (int c = 0; c < p; c++) v[i] = c, rec(i + 1);
                        };
                        rec(0);
                        CHECK_EQ(rk != -1, ex);
                }
                if (rk != -1) for (auto &r : s) { ll t = 0; for (int j = 0; j < m; j++) t += r[j] * x[j]; CHECK_EQ((t - r[m]) % p, 0); }
        }
        for (int it = 0; it < 1000; it++) {
                int n = rnd(1, 6);
                ve<ve<ld>> a(n, ve<ld>(n + 1));
                ve<ld> xs(n);
                for (auto &v : xs) v = rnd(-5, 5);
                for (auto &r : a) { for (int j = 0; j < n; j++) r[j] = rnd(-5, 5); r[n] = 0; for (int j = 0; j < n; j++) r[n] += r[j] * xs[j]; }
                ve<ld> x; int rk = gaussD(a, x);
                CHECK(rk >= 0);
                for (auto &r : a) { ld t = 0; for (int j = 0; j < n; j++) t += r[j] * x[j]; CHECK(abs(t - r[n]) < 1e-6); }
        }
        // матричная степень: Фибоначчи
        { Mat f = {{1, 1}, {1, 0}}; vll fib = {0, 1}; for (int i = 2; i < 60; i++) fib.push_back((fib[i - 1] + fib[i - 2]) % MOD);
          for (int i = 1; i < 59; i++) CHECK_EQ(matPow(f, i)[0][1], fib[i]); }
        // Берлекэмп-Мэсси / n-й член / Лагранж
        for (int it = 0; it < 2000; it++) {
                int d = rnd(1, 6);
                vll c(d), s(d);
                for (auto &x : c) x = rnd(0, MOD - 1);
                for (auto &x : s) x = rnd(0, MOD - 1);
                int N = 60;
                while ((int)s.size() < N) { ll t = 0; int i = s.size(); for (int j = 0; j < d; j++) t = (t + c[j] * s[i - 1 - j]) % MOD; s.push_back(t); }
                vll pre(s.begin(), s.begin() + 2 * d + 2);
                vll rc = berlekampMassey(pre);
                CHECK(rc.size() <= (size_t)d);
                for (int k = 0; k < N; k++) CHECK_EQ(nthTerm(vll(s.begin(), s.begin() + 2 * d + 2), rc, k), s[k]);
                // Лагранж
                int deg = rnd(0, 6); vll co(deg + 1);
                for (auto &x : co) x = rnd(0, MOD - 1);
                auto ev = [&](ll x) { ll r = 0; for (int i = deg; i >= 0; i--) r = (r * (x % MOD) + co[i]) % MOD; return r; };
                int n = deg + 1 + rnd(0, 3); vll y(n);
                for (int i = 0; i < n; i++) y[i] = ev(i);
                ll x = rnd(0, 1) ? rnd(0, 10) : rnd(0, (ll)4e18);
                CHECK_EQ(lagrange(y, x), ev(x));
        }
        // C, Лукас, Люси
        { int N = 300; Combi cb(N); ve<vll> pas(N + 1, vll(N + 1));
          for (int i = 0; i <= N; i++) { pas[i][0] = 1; for (int j = 1; j <= i; j++) pas[i][j] = (pas[i - 1][j - 1] + (j <= i - 1 ? pas[i - 1][j] : 0)) % MOD; }
          for (int i = 0; i <= N; i++) for (int j = 0; j <= i; j++) CHECK_EQ(cb.C(i, j), pas[i][j]);
          vll cat = {1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796}; for (int i = 0; i < 11; i++) CHECK_EQ(cb.catalan(i), cat[i]);
          for (int p : {2, 3, 5, 7, 13}) { ve<vll> q(301, vll(301)); for (int i = 0; i <= 300; i++) { q[i][0] = 1; for (int j = 1; j <= i; j++) q[i][j] = (q[i - 1][j - 1] + q[i - 1][j]) % p; }
                for (int n = 0; n <= 300; n++) for (int k = 0; k <= n; k++) CHECK_EQ(lucas(n, k, p), q[n][k]); } }
        { vi sv(2000001, 1); sv[0] = sv[1] = 0; for (int i = 2; i <= 2000000; i++) if (sv[i]) for (ll j = (ll)i * i; j <= 2000000; j += i) sv[j] = 0;
          vi pc(2000001); for (int i = 1; i <= 2000000; i++) pc[i] = pc[i - 1] + sv[i];
          for (int n = 0; n < 3000; n++) CHECK_EQ(primeCount(n), pc[n]);
          for (int it = 0; it < 200; it++) { int n = rnd(0, 2000000); CHECK_EQ(primeCount(n), pc[n]); }
          CHECK_EQ(primeCount(100000000LL), 5761455LL); CHECK_EQ(primeCount(1000000000LL), 50847534LL); CHECK_EQ(primeCount(10000000000LL), 455052511LL); }
        cout << "ok\n";
}
