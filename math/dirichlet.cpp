#pragma once
#include "math/numth.cpp"

// Суммы по делителям/кратным за O(n log log n) (по простым), индексы 1..n (f[0] не используется).
// zetaDiv:  g[m] = sum_{d | m} f[d];      mobDiv: обратная (Мёбиус по делителям)
// zetaMul:  g[d] = sum_{d | m} f[m];      mobMul: обратная (Мёбиус по кратным)
void zetaDiv(vll &f, const vi &primes) {
        int n = (int)f.size() - 1;
        for (int p : primes) for (int i = 1; i * p <= n; i++) f[i * p] += f[i];
}

void mobDiv(vll &f, const vi &primes) {
        int n = (int)f.size() - 1;
        for (int p : primes) for (int i = n / p; i >= 1; i--) f[i * p] -= f[i];
}

void zetaMul(vll &f, const vi &primes) {
        int n = (int)f.size() - 1;
        for (int p : primes) for (int i = n / p; i >= 1; i--) f[i] += f[i * p];
}

void mobMul(vll &f, const vi &primes) {
        int n = (int)f.size() - 1;
        for (int p : primes) for (int i = 1; i * p <= n; i++) f[i] -= f[i * p];
}

// c[k] = sum_{gcd(i, j) = k} a[i] b[j]
vll gcdConv(vll a, vll b, const vi &primes) {
        zetaMul(a, primes);
        zetaMul(b, primes);
        for (size_t i = 0; i < a.size(); i++) a[i] *= b[i];
        mobMul(a, primes);
        return a;
}

// c[k] = sum_{lcm(i, j) = k} a[i] b[j]  (пары с lcm > n отбрасываются)
vll lcmConv(vll a, vll b, const vi &primes) {
        zetaDiv(a, primes);
        zetaDiv(b, primes);
        for (size_t i = 0; i < a.size(); i++) a[i] *= b[i];
        mobDiv(a, primes);
        return a;
}

// Свёртка Дирихле c[n] = sum_{ij = n} a[i] b[j], O(n log n)
vll dirichlet(const vll &a, const vll &b) {
        int n = (int)a.size() - 1;
        vll c(n + 1, 0);
        for (int i = 1; i <= n; i++) for (int j = 1; i * j <= n; j++) c[i * j] += a[i] * b[j];
        return c;
}
