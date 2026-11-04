#pragma once
#include "tmp.cpp"

// z[i] = lcp(s, s[i..]), z[0] = 0
vi z_func(const string &s) {
        int n = s.length();
        vi z(n);
        int l = 0, r = 0;
        for (int i = 1; i < n; i++) {
                if (i < r) z[i] = min(r - i, z[i - l]);
                while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
                if (i + z[i] > r) l = i, r = i + z[i];
        }
        return z;
}

// p[i] = длина наибольшего собственного префикса-суффикса s[0..i]
vi pref_func(const string &s) {
        int n = s.length();
        vi p(n);
        for (int i = 1; i < n; i++) {
                int j = p[i - 1];
                while (j > 0 && s[j] != s[i]) j = p[j - 1];
                if (s[i] == s[j]) j++;
                p[i] = j;
        }
        return p;
}

// Манакер: d1[i] -- число нечётных палиндромов с центром i (радиус), d2[i] -- число чётных
// палиндромов с центром между i - 1 и i. O(n)
pair<vi, vi> manacher(const string &s) {
        int n = s.size();
        vi d1(n), d2(n);
        for (int i = 0, l = 0, r = -1; i < n; i++) {
                int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
                while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
                d1[i] = k--;
                if (i + k > r) l = i - k, r = i + k;
        }
        for (int i = 0, l = 0, r = -1; i < n; i++) {
                int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
                while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
                d2[i] = k--;
                if (i + k > r) l = i - k - 1, r = i + k;
        }
        return {d1, d2};
}

// Начало лексикографически минимального циклического сдвига, O(n)
int minRotation(const string &s) {
        int n = s.size(), i = 0, j = 1, k = 0;
        while (i < n && j < n && k < n) {
                char a = s[(i + k) % n], b = s[(j + k) % n];
                if (a == b) {
                        k++;
                        continue;
                }
                if (a > b) i += k + 1;
                else j += k + 1;
                if (i == j) j++;
                k = 0;
        }
        return min(i, j);
}

// Полиномиальный хеш по модулю 2^61 - 1 со случайной базой. get(l, r) -- хеш s[l, r)
struct Hash61 {
        static const ull M = (1ULL << 61) - 1;
        vector<ull> h, pw;

        static ull mul(ull a, ull b) {
                unsigned __int128 t = (unsigned __int128)a * b;
                ull r = (ull)(t & M) + (ull)(t >> 61);
                return r >= M ? r - M : r;
        }

        Hash61(const string &s) : h(s.size() + 1), pw(s.size() + 1) {
                static const ull base = mt19937_64(chrono::steady_clock::now().time_since_epoch().count())() % (M - 1000) + 500;
                pw[0] = 1;
                for (size_t i = 0; i < s.size(); i++) {
                        pw[i + 1] = mul(pw[i], base);
                        h[i + 1] = mul(h[i], base) + (ull)s[i];
                        if (h[i + 1] >= M) h[i + 1] -= M;
                }
        }

        ull get(int l, int r) {
                ull res = h[r] + M - mul(h[l], pw[r - l]);
                return res >= M ? res - M : res;
        }
};

// Суффиксный массив (удвоение + сортировка подсчётом), O(n log n). Возвращает sa.
vi suffixArray(string s) {
        s += char(1);
        int n = s.size(), cls = 1;
        vi p(n), c(n), cnt(max(256, n), 0);
        for (char ch : s) cnt[(unsigned char)ch]++;
        for (int i = 1; i < 256; i++) cnt[i] += cnt[i - 1];
        for (int i = 0; i < n; i++) p[--cnt[(unsigned char)s[i]]] = i;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) {
                if (s[p[i]] != s[p[i - 1]]) cls++;
                c[p[i]] = cls - 1;
        }
        vi pn(n), cn(n);
        for (int h = 0; (1 << h) < n; h++) {
                for (int i = 0; i < n; i++) {
                        pn[i] = p[i] - (1 << h);
                        if (pn[i] < 0) pn[i] += n;
                }
                fill(cnt.begin(), cnt.begin() + cls, 0);
                for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
                for (int i = 1; i < cls; i++) cnt[i] += cnt[i - 1];
                for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
                cn[p[0]] = 0;
                cls = 1;
                for (int i = 1; i < n; i++) {
                        pii cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
                        pii prv = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
                        if (cur != prv) cls++;
                        cn[p[i]] = cls - 1;
                }
                c.swap(cn);
        }
        p.erase(p.begin());
        return p;
}

// lcp[i] = lcp(suffix sa[i], suffix sa[i + 1]) (Касай), размер n - 1
vi lcpArray(const string &s, const vi &sa) {
        int n = s.size(), k = 0;
        vi rk(n), lcp(max(n - 1, 0));
        for (int i = 0; i < n; i++) rk[sa[i]] = i;
        for (int i = 0; i < n; i++) {
                if (rk[i] == n - 1) {
                        k = 0;
                        continue;
                }
                int j = sa[rk[i] + 1];
                while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
                lcp[rk[i]] = k;
                if (k) k--;
        }
        return lcp;
}
