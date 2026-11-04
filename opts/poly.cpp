#pragma once
#include "opts/ntt.cpp"

// Многочлены по модулю 998244353 (вектор коэффициентов по возрастанию степени).
const int PM = 998244353;
using poly = ve<int>;

poly polyMul(const poly &a, const poly &b) {
        return conv(a, b);
}

poly polyInv(const poly &a, int n) {  // 1 / a mod x^n, a[0] != 0; Ньютон: b = b (2 - a b)
        poly b = {(int)inv(a[0], PM)};
        for (int m = 1; m < n; m *= 2) {
                poly t(a.begin(), a.begin() + min((int)a.size(), 2 * m));
                poly ab = conv(t, b);
                ab.resize(2 * m);
                for (auto &x : ab) x = (x == 0) ? 0 : PM - x;
                ab[0] = (ab[0] + 2) % PM;
                b = conv(b, ab);
                b.resize(2 * m);
        }
        b.resize(n);
        return b;
}

poly polyDer(const poly &a) {
        poly r(max<int>(0, (int)a.size() - 1));
        for (size_t i = 1; i < a.size(); i++) r[i - 1] = (ll)a[i] * i % PM;
        return r;
}

poly polyInt(const poly &a) {
        poly r(a.size() + 1);
        for (size_t i = 0; i < a.size(); i++) r[i + 1] = a[i] * inv(i + 1, PM) % PM;
        return r;
}

poly polyLog(const poly &a, int n) {  // ln a mod x^n, a[0] = 1
        poly r = conv(polyDer(a), polyInv(a, n));
        r.resize(n - 1);
        r = polyInt(r);
        r.resize(n);
        return r;
}

poly polyExp(const poly &a, int n) {  // exp a mod x^n, a[0] = 0; Ньютон: b = b (1 - ln b + a)
        poly b = {1};
        for (int m = 1; m < n; m *= 2) {
                poly l = polyLog(b, 2 * m);
                for (int i = 0; i < 2 * m; i++) {
                        int ai = i < (int)a.size() ? a[i] : 0;
                        l[i] = ((ai - l[i]) % PM + PM) % PM;
                }
                l[0] = (l[0] + 1) % PM;
                b = conv(b, l);
                b.resize(2 * m);
        }
        b.resize(n);
        return b;
}

// c(n, k): коэффициенты x (x + 1) ... (x + n - 1) -- беззнаковые числа Стирлинга I рода, D&C + NTT
poly prodLin(int l, int r) {  // prod_{t = l}^{r - 1} (x + t)
        if (r - l == 1) return {l % PM, 1};
        int m = (l + r) / 2;
        return conv(prodLin(l, m), prodLin(m, r));
}

poly stirling1Row(int n) {
        return n == 0 ? poly{1} : prodLin(0, n);
}

// S(n, k), k = 0..n : [x^k] (sum i^n x^i / i!) (sum (-1)^j x^j / j!)
poly stirling2Row(int n) {
        ve<ll> f(n + 1, 1);
        for (int i = 1; i <= n; i++) f[i] = f[i - 1] * i % PM;
        poly a(n + 1), b(n + 1);
        for (int i = 0; i <= n; i++) {
                ll fi = inv(f[i], PM);
                a[i] = binpow(i, n, PM) * fi % PM;
                b[i] = (i & 1) ? (PM - fi) % PM : fi;
        }
        poly r = conv(a, b);
        r.resize(n + 1);
        return r;
}
