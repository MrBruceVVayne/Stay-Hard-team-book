#pragma once
#include "tmp.cpp"

using cd = complex<double>;
const double PI = acos(-1.0);

void fft(ve<cd> &a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
                int bit = n >> 1;
                for (; j & bit; bit >>= 1) j ^= bit;
                j ^= bit;
                if (i < j) swap(a[i], a[j]);
        }
        ve<cd> rt(n / 2 + 1);
        for (int i = 0; i < n / 2; i++) rt[i] = polar(1.0, 2 * PI * i / n * (invert ? -1 : 1));
        for (int len = 2; len <= n; len <<= 1) {
                int step = n / len;
                for (int i = 0; i < n; i += len) {
                        for (int j = 0; j < len / 2; j++) {
                                cd u = a[i + j], v = a[i + j + len / 2] * rt[j * step];
                                a[i + j] = u + v;
                                a[i + j + len / 2] = u - v;
                        }
                }
        }
        if (invert) for (auto &x : a) x /= n;
}

// Точная целочисленная свёртка, пока |ответ| <~ 1e14 / log n (для больших -- разбивать на 15 бит)
ve<ll> convFFT(const ve<ll> &a, const ve<ll> &b) {
        int need = a.size() + b.size() - 1, n = 1;
        while (n < need) n <<= 1;
        ve<cd> fa(all(a)), fb(all(b));
        fa.resize(n), fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true);
        ve<ll> res(need);
        for (int i = 0; i < need; i++) res[i] = llround(fa[i].real());
        return res;
}
