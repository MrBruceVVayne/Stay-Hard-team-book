#pragma once
#include "math/numth.cpp"

// Берлекэмп-Мэсси по модулю MOD (простой): s[i] = sum_j c[j] * s[i - 1 - j].
vll berlekampMassey(const vll &s) {
        int n = s.size(), L = 0, m = 0;
        vll C(n), B(n), T;
        C[0] = B[0] = 1;
        ll b = 1;
        for (int i = 0; i < n; i++) {
                m++;
                ll d = s[i] % MOD;
                for (int j = 1; j <= L; j++) d = (d + C[j] * s[i - j]) % MOD;
                if (!d) continue;
                T = C;
                ll coef = d * inv(b) % MOD;
                for (int j = m; j < n; j++) C[j] = ((C[j] - coef * B[j - m]) % MOD + MOD) % MOD;
                if (2 * L > i) continue;
                L = i + 1 - L, B = T, b = d, m = 0;
        }
        C.resize(L + 1);
        C.erase(C.begin());
        for (auto &x : C) x = (MOD - x) % MOD;
        return C;
}

// k-й член (с нуля) по рекурренте c и начальным s: x^k mod характеристический многочлен. O(n^2 log k)
ll nthTerm(const vll &s, const vll &c, ll k) {
        int n = c.size();
        if (k < (ll)s.size()) return s[k];
        auto mul = [&](const vll &a, const vll &b) {
                vll r(2 * n, 0);
                for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) r[i + j] = (r[i + j] + a[i] * b[j]) % MOD;
                for (int i = 2 * n - 1; i >= n; i--) {
                        for (int j = 0; j < n; j++) r[i - 1 - j] = (r[i - 1 - j] + r[i] * c[j]) % MOD;
                }
                r.resize(n);
                return r;
        };
        vll res(n, 0), base(n, 0);
        res[0] = 1;
        if (n == 1) base[0] = c[0];
        else base[1] = 1;
        for (; k; k >>= 1, base = mul(base, base)) if (k & 1) res = mul(res, base);
        ll ans = 0;
        for (int i = 0; i < n; i++) ans = (ans + res[i] * s[i]) % MOD;
        return ans;
}

// y[i] = f(i), i = 0..n-1, deg f < n. f(x) mod MOD за O(n)
ll lagrange(const vll &y, ll x) {
        int n = y.size();
        x %= MOD;
        if (x < n) return y[x];
        vll pre(n + 1, 1), suf(n + 1, 1), fact(n, 1);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] * ((x - i + MOD) % MOD) % MOD;
        for (int i = n - 1; i >= 0; i--) suf[i] = suf[i + 1] * ((x - i + MOD) % MOD) % MOD;
        for (int i = 1; i < n; i++) fact[i] = fact[i - 1] * i % MOD;
        ll res = 0;
        for (int i = 0; i < n; i++) {
                ll term = y[i] * pre[i] % MOD * suf[i + 1] % MOD;
                term = term * inv(fact[i] * fact[n - 1 - i] % MOD) % MOD;
                if ((n - 1 - i) & 1) term = (MOD - term) % MOD;
                res = (res + term) % MOD;
        }
        return res;
}
