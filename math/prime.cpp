#pragma once
#include "tmp.cpp"

ull mulmod(ull a, ull b, ull m) {
        return (unsigned __int128)a * b % m;
}

ull powmod(ull a, ull b, ull m) {
        ull r = 1 % m;
        for (; b; b >>= 1, a = mulmod(a, a, m)) {
                if (b & 1) r = mulmod(r, a, m);
        }
        return r;
}

// Детерминированный Миллер-Рабин для всех n < 2^64
bool isPrime(ull n) {
        if (n < 2) return false;
        for (ull p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                if (n % p == 0) return n == p;
        }
        ull d = n - 1;
        int s = 0;
        while (d % 2 == 0) d /= 2, s++;
        for (ull a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                ull x = powmod(a, d, n);
                if (x == 1 || x == n - 1) continue;
                bool comp = true;
                for (int i = 1; i < s && comp; i++) {
                        x = mulmod(x, x, n);
                        if (x == n - 1) comp = false;
                }
                if (comp) return false;
        }
        return true;
}

// Ро-Поллард (Флойд, gcd раз в 64 шага; при g = n откат и gcd на каждом шаге)
ull rho(ull n) {
        if (n % 2 == 0) return 2;
        static mt19937_64 gen(2024);
        while (true) {
                ull c = gen() % (n - 1) + 1, x = gen() % n, y = x, g = 1;
                auto f = [&](ull v) {
                        ull r = mulmod(v, v, n) + c;
                        if (r < c || r >= n) r -= n;
                        return r;
                };
                auto step = [&]() {
                        x = f(x);
                        y = f(f(y));
                        return x > y ? x - y : y - x;
                };
                while (g == 1) {
                        ull sx = x, sy = y, prod = 1;
                        for (int i = 0; i < 64; i++) prod = mulmod(prod, step(), n);
                        g = __gcd(prod, n);
                        if (g == n) {
                                x = sx, y = sy;
                                for (g = 1; g == 1;) g = __gcd(step(), n);  // d = 0 -> gcd = n
                        }
                }
                if (g != n) return g;
        }
}

void factorRec(ull n, ve<ull> &out) {
        if (n == 1) return;
        if (isPrime(n)) {
                out.push_back(n);
                return;
        }
        ull d = rho(n);
        factorRec(d, out);
        factorRec(n / d, out);
}

ve<ull> factor(ull n) {  // простые делители с кратностью, по возрастанию
        ve<ull> res;
        for (ull p = 2; p < 100 && p * p <= n; p++) {
                while (n % p == 0) res.push_back(p), n /= p;
        }
        factorRec(n, res);
        sort(all(res));
        return res;
}
