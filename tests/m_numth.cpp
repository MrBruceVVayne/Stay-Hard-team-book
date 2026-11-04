#include "common.h"
#include "math/numth.cpp"
#include "math/prime.cpp"
#include "math/modular.cpp"

ll gcdb(ll a, ll b) { return b ? gcdb(b, a % b) : a; }

signed main() {
        // binpow / inv / extgcd / diophant
        for (int it = 0; it < 20000; it++) {
                ll m = rnd(1, 1000), a = rnd(-2000, 2000), b = rnd(0, 40);
                ll r = 1 % m; for (int i = 0; i < b; i++) r = ((r * a) % m + m) % m;
                CHECK_EQ(binpow(a, b, m), r);
                ll x, y;
                ll g = extgcd(abs(a), m, x, y);
                CHECK_EQ(abs(a) * x + m * y, g);
                if (gcdb(abs(a), m) == 1 && m > 1) CHECK_EQ(inv(a, m) * (((a % m) + m) % m) % m, 1);
                ll A = rnd(-30, 30), B = rnd(-30, 30), C = rnd(-100, 100);
                if (A == 0 && B == 0) continue;
                bool ok = diophant(A, B, C, x, y);
                bool exp = false;
                for (ll u = -110; u <= 110 && !exp; u++) { ll rest = C - A * u; if (B == 0 ? rest == 0 : rest % B == 0) exp = true; }
                CHECK_EQ(ok, exp);
                if (ok) CHECK_EQ(A * x + B * y, C);
        }
        // CRT
        for (int it = 0; it < 20000; it++) {
                ll m1 = rnd(1, 60), m2 = rnd(1, 60), a1 = rnd(0, m1 - 1), a2 = rnd(0, m2 - 1);
                auto [x, l] = crt(a1, m1, a2, m2);
                ll exp = -1;
                for (ll v = 0; v < m1 / gcdb(m1, m2) * m2; v++) if (v % m1 == a1 && v % m2 == a2) { exp = v; break; }
                CHECK_EQ(x, exp);
        }
        // решето, phi, mu
        Sieve s(3000);
        for (int n = 1; n <= 3000; n++) {
                int ph = 0, cnt = 0; ll nn = n;
                for (int k = 1; k <= n; k++) ph += gcdb(k, n) == 1;
                int mu = 1;
                for (int p = 2; p <= nn; p++) if (nn % p == 0) { int c = 0; while (nn % p == 0) nn /= p, c++; mu = c > 1 ? 0 : -mu; }
                CHECK_EQ(s.phi[n], ph); CHECK_EQ(s.mu[n], mu); CHECK_EQ((int)phiFunc(n), ph);
                bool pr = n > 1; for (int d = 2; d * d <= n; d++) if (n % d == 0) pr = false;
                CHECK_EQ(isPrime(n), pr); CHECK_EQ(s.lp[n] == n, pr);
        }
        // псевдопростые и большие простые
        for (ull c : {3215031751ULL, 2152302898747ULL, 3474749660383ULL, 341550071728321ULL, 3825123056546413051ULL, 318665857834031151167461ULL % 1000000007ULL}) {
                bool pr = c > 1; for (ull d = 2; d * d <= c && d < 5000000; d++) if (c % d == 0) { pr = false; break; }
                if (c < 25000000000000ULL) CHECK_EQ(isPrime(c), pr);
        }
        CHECK(isPrime(1000000007) && isPrime(998244353) && isPrime(1000000000000000003ULL) && isPrime(18446744073709551557ULL));
        CHECK(!isPrime(3825123056546413051ULL) && !isPrime(1000000007ULL * 998244353ULL));
        // факторизация
        for (int it = 0; it < 3000; it++) {
                ull n = rnd(1, 4000000000LL);
                ve<ull> f = factor(n), e; ull nn = n;
                for (ull p = 2; p * p <= nn; p++) while (nn % p == 0) e.push_back(p), nn /= p;
                if (nn > 1) e.push_back(nn);
                CHECK_EQ(f, e);
        }
        for (int it = 0; it < 300; it++) {
                ull n = (ull)rnd(1, (1LL << 62));
                ve<ull> f = factor(n); ull pr = 1;
                for (ull p : f) { CHECK(isPrime(p)); pr *= p; }
                CHECK_EQ(pr, n);
        }
        ull big = 999999937ULL * 999999929ULL; auto f = factor(big);
        CHECK_EQ(f, (ve<ull>{999999929ULL, 999999937ULL}));
        // sqrt mod, первообразный корень
        for (ll p : {2, 3, 5, 7, 13, 17, 97, 101, 193, 257, 1009, 7681, 12289}) {
                for (ll x = 0; x < p; x++) {
                        ll r = sqrtMod(x * x % p, p);
                        CHECK(r != -1 && r * r % p == x * x % p);
                }
                if (p > 2) { int sq = 0; for (ll x = 1; x < p; x++) if (sqrtMod(x, p) != -1) sq++; CHECK_EQ(sq, (int)((p - 1) / 2)); }
                ll g = primRoot(p); set<ll> seen; ll v = 1;
                for (int i = 0; i < p - 1; i++) { seen.insert(v); v = v * g % p; }
                CHECK_EQ((ll)seen.size(), p - 1);
        }
        CHECK_EQ(sqrtMod(10, 13) * sqrtMod(10, 13) % 13, 10);
        { ll p = 998244353; for (int i = 0; i < 100; i++) { ll x = rnd(1, p - 1); ll r = sqrtMod(x * x % p, p); CHECK_EQ(r * r % p, x * x % p); } }
        // модульный логарифм
        for (int it = 0; it < 30000; it++) {
                ll m = rnd(1, 80), a = rnd(0, 200), b = rnd(0, 200), exp = -1;
                ll cur = 1 % m;
                for (int x = 0; x < 400; x++) { if (cur == b % m) { exp = x; break; } cur = cur * (a % m) % m; }
                CHECK_EQ(modlog(a, b, m), exp);
        }
        // floor sum
        for (int it = 0; it < 30000; it++) {
                ll n = rnd(1, 40), m = rnd(1, 40), a = rnd(0, 100), b = rnd(0, 100), s2 = 0;
                for (ll i = 0; i < n; i++) s2 += (a * i + b) / m;
                CHECK_EQ(floorSum(n, m, a, b), s2);
        }
        cout << "ok\n";
}
