#pragma once
#include "tmp.cpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

// ordered set: order_of_key(x) -- число элементов < x, find_by_order(k) -- k-й (с нуля).
// Для мультимножества: пары (значение, уникальный номер).
using namespace __gnu_pbds;
template <typename T>
using oset = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// Защита от антихеш-тестов: unordered_map<ll, T, SplitMix> (или gp_hash_table)
struct SplitMix {
        size_t operator()(ull x) const {
                static const ull FIXED = chrono::steady_clock::now().time_since_epoch().count();
                x += FIXED + 0x9e3779b97f4a7c15ULL;
                x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
                x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
                return x ^ (x >> 31);
        }
};

// Быстрый ввод целых (fread-буфер)
static char ibuf[1 << 25];
size_t ipos = 0, ilen = 0;
inline int gc() {
        if (ipos == ilen) {
                ilen = fread(ibuf, 1, sizeof(ibuf), stdin);
                ipos = 0;
                if (!ilen) return -1;
        }
        return ibuf[ipos++];
}

inline ll readInt() {
        int c = gc();
        while (c != '-' && (c < '0' || c > '9')) c = gc();
        bool neg = (c == '-');
        if (neg) c = gc();
        ll x = 0;
        for (; c >= '0' && c <= '9'; c = gc()) x = x * 10 + (c - '0');
        return neg ? -x : x;
}

// Вывод __int128
string toString(__int128 x) {
        if (x == 0) return "0";
        bool neg = x < 0;
        if (neg) x = -x;
        string s;
        for (; x; x /= 10) s += char('0' + (int)(x % 10));
        if (neg) s += '-';
        reverse(all(s));
        return s;
}

mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());
