#pragma once
#include "math/numth.cpp"

// NTT по модулю простого M = c * 2^k + 1 с первообразным корнем G.
// 998244353 (g = 3), 167772161 (g = 3), 469762049 (g = 3).
template<int M, int G>
void ntt(ve<int> &a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
                int bit = n >> 1;
                for (; j & bit; bit >>= 1) j ^= bit;
                j ^= bit;
                if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
                ll w = binpow(G, (M - 1) / len, M);
                if (invert) w = binpow(w, M - 2, M);
                for (int i = 0; i < n; i += len) {
                        ll cur = 1;
                        for (int j = 0; j < len / 2; j++) {
                                int u = a[i + j], v = a[i + j + len / 2] * cur % M;
                                a[i + j] = (u + v >= M) ? u + v - M : u + v;
                                a[i + j + len / 2] = (u - v < 0) ? u - v + M : u - v;
                                cur = cur * w % M;
                        }
                }
        }
        if (invert) {
                ll ni = binpow(n, M - 2, M);
                for (auto &x : a) x = x * ni % M;
        }
}

template<int M, int G>
ve<int> convNTT(ve<int> a, ve<int> b) {
        int need = a.size() + b.size() - 1, n = 1;
        while (n < need) n <<= 1;
        a.resize(n), b.resize(n);
        for (auto &x : a) x %= M;
        for (auto &x : b) x %= M;
        ntt<M, G>(a, false);
        ntt<M, G>(b, false);
        for (int i = 0; i < n; i++) a[i] = (ll)a[i] * b[i] % M;
        ntt<M, G>(a, true);
        a.resize(need);
        return a;
}

ve<int> conv(const ve<int> &a, const ve<int> &b) {  // mod 998244353
        return convNTT<998244353, 3>(a, b);
}

// Свёртка по произвольному модулю mod (< 2^31): три NTT-модуля + Гарнер.
// Коэффициенты в [0, mod), длина до ~ 1e6.
ve<int> convMod(const ve<int> &a, const ve<int> &b, int mod) {
        const int m1 = 998244353, m2 = 167772161, m3 = 469762049;
        auto r1 = convNTT<m1, 3>(a, b), r2 = convNTT<m2, 3>(a, b), r3 = convNTT<m3, 3>(a, b);
        ll i12 = inv(m1, m2), i123 = inv((ll)m1 * m2 % m3, m3);
        ve<int> res(r1.size());
        for (size_t i = 0; i < res.size(); i++) {
                ll x2 = ((r2[i] - r1[i]) % m2 + m2) % m2 * i12 % m2;
                ll x12 = r1[i] + x2 * m1;  // < m1 * m2
                ll x3 = ((r3[i] - x12 % m3) % m3 + m3) % m3 * i123 % m3;
                res[i] = (x12 % mod + x3 * ((ll)m1 * m2 % mod)) % mod;
        }
        return res;
}
