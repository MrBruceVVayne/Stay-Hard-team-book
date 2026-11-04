#pragma once
#include "math/numth.cpp"

// sqrt(a) mod p, p простое (Тонелли-Шенкс), -1 если корня нет
ll sqrtMod(ll a, ll p) {
        a %= p;
        if (a < 0) a += p;
        if (a == 0 || p == 2) return a;
        if (binpow(a, (p - 1) / 2, p) != 1) return -1;
        if (p % 4 == 3) return binpow(a, (p + 1) / 4, p);
        ll q = p - 1;
        int s = 0;
        while (q % 2 == 0) q /= 2, s++;
        ll z = 2;
        while (binpow(z, (p - 1) / 2, p) != p - 1) z++;
        ll c = binpow(z, q, p), x = binpow(a, (q + 1) / 2, p), t = binpow(a, q, p);
        int m = s;
        while (t != 1) {
                int i = 0;
                for (ll tt = t; tt != 1; tt = tt * tt % p) i++;
                ll b = c;
                for (int j = 0; j < m - i - 1; j++) b = b * b % p;
                x = x * b % p;
                c = b * b % p;
                t = t * c % p;
                m = i;
        }
        return x;
}

// Первообразный корень по простому p
ll primRoot(ll p) {
        if (p == 2) return 1;
        ve<ll> fs;
        ll n = p - 1;
        for (ll d = 2; d * d <= n; d++) {
                if (n % d) continue;
                fs.push_back(d);
                while (n % d == 0) n /= d;
        }
        if (n > 1) fs.push_back(n);
        for (ll g = 2;; g++) {
                bool ok = true;
                for (ll f : fs) ok &= binpow(g, (p - 1) / f, p) != 1;
                if (ok) return g;
        }
}

// min x >= 0: a^x = b (mod m), любой m; -1 если нет. O(sqrt m log m)
ll modlog(ll a, ll b, ll m) {
        a %= m, b %= m;
        ll k = 1 % m, add = 0, g;
        while ((g = __gcd(a, m)) > 1) {
                if (b == k) return add;
                if (b % g) return -1;
                b /= g, m /= g, add++;
                k = k * (a / g) % m;
        }
        ll n = (ll)sqrt((ld)m) + 1, an = 1;
        for (ll i = 0; i < n; i++) an = an * a % m;
        map<ll, ll> vals;
        for (ll q = 0, cur = b; q <= n; q++, cur = cur * a % m) vals[cur] = q;
        for (ll p = 1, cur = k; p <= n; p++) {
                cur = cur * an % m;
                auto it = vals.find(cur);
                if (it != vals.end()) return n * p - it->second + add;
        }
        return -1;
}

// sum_{i=0}^{n-1} floor((a * i + b) / m), a, b >= 0
ll floorSum(ll n, ll m, ll a, ll b) {
        ll res = 0;
        while (true) {
                if (a >= m) res += (n - 1) * n / 2 * (a / m), a %= m;
                if (b >= m) res += n * (b / m), b %= m;
                ll y = a * n + b;
                if (y < m) break;
                n = y / m;
                b = y % m;
                swap(m, a);
        }
        return res;
}
