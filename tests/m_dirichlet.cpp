#include "common.h"
#include "math/dirichlet.cpp"

signed main() {
        for (int it = 0; it < 2000; it++) {
                int n = rnd(1, 80);
                Sieve s(n);
                vll a(n + 1), b(n + 1);
                for (int i = 1; i <= n; i++) a[i] = rnd(-5, 5), b[i] = rnd(-5, 5);
                vll z = a, zm = a, g(n + 1), m(n + 1), gc(n + 1), lc(n + 1), dc(n + 1);
                zetaDiv(z, s.primes); zetaMul(zm, s.primes);
                for (int x = 1; x <= n; x++) for (int d = 1; d <= n; d++) {
                        if (x % d == 0) g[x] += a[d];        // сумма по делителям x
                        if (d % x == 0) m[x] += a[d];        // сумма по кратным x
                }
                CHECK_EQ(z, vll(g.begin(), g.end()));
                CHECK_EQ(zm, vll(m.begin(), m.end()));
                mobDiv(z, s.primes); mobMul(zm, s.primes);
                CHECK_EQ(z, a); CHECK_EQ(zm, a);
                for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) {
                        gc[__gcd(i, j)] += a[i] * b[j];
                        if ((ll)i / __gcd(i, j) * j <= n) lc[i / __gcd(i, j) * j] += a[i] * b[j];
                        if ((ll)i * j <= n) dc[i * j] += a[i] * b[j];
                }
                CHECK_EQ(gcdConv(a, b, s.primes), gc);
                CHECK_EQ(lcmConv(a, b, s.primes), lc);
                CHECK_EQ(dirichlet(a, b), dc);
                // mu * 1 = [n = 1]; phi * 1 = id
                vll mu(n + 1), one(n + 1, 1), ph(n + 1), id(n + 1);
                one[0] = 0; for (int i = 1; i <= n; i++) mu[i] = s.mu[i], ph[i] = s.phi[i], id[i] = i;
                vll e = dirichlet(mu, one), p1 = dirichlet(ph, one);
                for (int i = 1; i <= n; i++) { CHECK_EQ(e[i], (ll)(i == 1)); CHECK_EQ(p1[i], id[i]); }
        }
        cout << "ok\n";
}
