#pragma once
#include "math/numth.cpp"

// Факториалы по модулю MOD до n. C, A (размещения), Каталан.
struct Combi {
        vll f, fi;

        Combi(int n) : f(n + 1), fi(n + 1) {
                f[0] = 1;
                for (int i = 1; i <= n; i++) f[i] = f[i - 1] * i % MOD;
                fi[n] = inv(f[n]);
                for (int i = n; i > 0; i--) fi[i - 1] = fi[i] * i % MOD;
        }

        ll C(int n, int k) {
                if (k < 0 || k > n) return 0;
                return f[n] * fi[k] % MOD * fi[n - k] % MOD;
        }

        ll A(int n, int k) {
                if (k < 0 || k > n) return 0;
                return f[n] * fi[n - k] % MOD;
        }

        ll catalan(int n) {
                return C(2 * n, n) * inv(n + 1) % MOD;
        }
};

// C(n, k) mod p для малого простого p (Лукас): O(p + log_p n)
ll lucas(ll n, ll k, int p) {
        static vll f;
        static int fp = 0;
        if (fp != p) {
                f.assign(p, 1);
                for (int i = 1; i < p; i++) f[i] = f[i - 1] * i % p;
                fp = p;
        }
        ll res = 1;
        for (; n || k; n /= p, k /= p) {
                int a = n % p, b = k % p;
                if (b > a) return 0;
                res = res * f[a] % p * inv(f[b] * f[a - b] % p, p) % p;
        }
        return res;
}

// Число простых <= n (Люси-Хаджеаз), O(n^{3/4})
ll primeCount(ll n) {
        if (n < 2) return 0;
        ll r = sqrtl(n);
        while (r * r > n) r--;
        while ((r + 1) * (r + 1) <= n) r++;
        vll lo(r + 2), hi(r + 2);
        for (ll i = 1; i <= r; i++) lo[i] = i - 1, hi[i] = n / i - 1;
        for (ll p = 2; p <= r; p++) {
                if (lo[p] == lo[p - 1]) continue;
                ll pc = lo[p - 1], p2 = p * p, end = min(r, n / p2);
                for (ll i = 1; i <= end; i++) {
                        ll d = i * p;
                        hi[i] -= (d <= r ? hi[d] : lo[n / d]) - pc;
                }
                for (ll i = r; i >= p2; i--) lo[i] -= lo[i / p] - pc;
        }
        return hi[1];
}
