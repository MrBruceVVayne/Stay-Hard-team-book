#pragma once
#include "tmp.cpp"

// Преобразование Уолша-Адамара. type: 0 -- xor, 1 -- and (суперсеты), 2 -- or (подмножества).
// Свёртка: f(a), f(b) -> покомпонентное произведение -> f^{-1} (invert = true). Длина = 2^k.
void fwht(vll &a, int type, bool invert) {
        int n = a.size();
        for (int len = 1; len < n; len <<= 1) {
                for (int i = 0; i < n; i += 2 * len) {
                        for (int j = i; j < i + len; j++) {
                                ll u = a[j], v = a[j + len];
                                if (type == 0) a[j] = u + v, a[j + len] = u - v;
                                else if (type == 1) a[j] = invert ? u - v : u + v;
                                else a[j + len] = invert ? v - u : u + v;
                        }
                }
        }
        if (type == 0 && invert) for (auto &x : a) x /= n;
}

vll xorConv(vll a, vll b, int type = 0) {
        fwht(a, type, false);
        fwht(b, type, false);
        for (size_t i = 0; i < a.size(); i++) a[i] *= b[i];
        fwht(a, type, true);
        return a;
}

// SOS-dp: f[mask] = сумма a[sub] по подмножествам; обратное -- вычитание
void sosSum(vll &f, int n) {
        for (int i = 0; i < n; i++) {
                for (int mask = 0; mask < (1 << n); mask++) {
                        if (mask >> i & 1) f[mask] += f[mask ^ (1 << i)];
                }
        }
}

void sosInv(vll &f, int n) {
        for (int i = 0; i < n; i++) {
                for (int mask = 0; mask < (1 << n); mask++) {
                        if (mask >> i & 1) f[mask] -= f[mask ^ (1 << i)];
                }
        }
}
