#pragma once
#include "tmp.cpp"

ll binpow(ll a, ll b, ll m = MOD) {  // a * a должно влезать в ll (m < 3e9)
        ll r = 1 % m;
        for (a %= m, a += (a < 0) * m; b; b >>= 1, a = a * a % m) {
                if (b & 1) r = r * a % m;
        }
        return r;
}

ll extgcd(ll a, ll b, ll &x, ll &y) {  // a * x + b * y = gcd(a, b)
        if (!b) {
                x = 1, y = 0;
                return a;
        }
        ll g = extgcd(b, a % b, y, x);
        y -= a / b * x;
        return g;
}

ll inv(ll a, ll m = MOD) {  // обратный по любому модулю, gcd(a, m) = 1
        ll x, y;
        extgcd((a % m + m) % m, m, x, y);
        return (x % m + m) % m;
}

// a * x + b * y = c. Возвращает false, если решений нет. Остальные: (x + k * b / g, y - k * a / g).
bool diophant(ll a, ll b, ll c, ll &x, ll &y) {
        ll g = extgcd(a, b, x, y);
        if (c % g) return false;
        x *= c / g;
        y *= c / g;
        return true;
}

// x = a1 (mod m1), x = a2 (mod m2) -> (x mod lcm, lcm) или (-1, -1)
pll crt(ll a1, ll m1, ll a2, ll m2) {
        ll x, y, g = extgcd(m1, m2, x, y);
        if ((a2 - a1) % g) return {-1, -1};
        ll l = m1 / g * m2, m = m2 / g;
        ll k = ((a2 - a1) / g % m + m) % m * (x % m + m) % m % m;
        ll res = (ll)(((__int128)m1 * k + a1) % l);
        return {(res + l) % l, l};
}

struct Sieve {  // линейное решето: lp, простые, phi, mu
        vi lp, primes, phi, mu;

        Sieve(int n) : lp(n + 1), phi(n + 1), mu(n + 1) {
                phi[1] = mu[1] = 1;
                for (int i = 2; i <= n; i++) {
                        if (!lp[i]) {
                                lp[i] = i;
                                phi[i] = i - 1;
                                mu[i] = -1;
                                primes.push_back(i);
                        }
                        for (int p : primes) {
                                if (p > lp[i] || (ll)i * p > n) break;
                                lp[i * p] = p;
                                if (i % p == 0) {
                                        phi[i * p] = phi[i] * p;
                                        mu[i * p] = 0;
                                } else {
                                        phi[i * p] = phi[i] * (p - 1);
                                        mu[i * p] = -mu[i];
                                }
                        }
                }
        }
};

ll phiFunc(ll n) {  // O(sqrt n)
        ll res = n;
        for (ll p = 2; p * p <= n; p++) {
                if (n % p) continue;
                while (n % p == 0) n /= p;
                res -= res / p;
        }
        if (n > 1) res -= res / n;
        return res;
}
