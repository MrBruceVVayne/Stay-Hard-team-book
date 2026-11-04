// Общий заголовок стресс-тестов: подключает шаблон без его main, даёт рандом и check.
#pragma once
#define main tmpl_main
#include "tmp.cpp"
#undef main

mt19937_64 trng(chrono::steady_clock::now().time_since_epoch().count() ^ 0x9e3779b97f4a7c15ULL);
ll rnd(ll l, ll r) { return l + (ll)(trng() % (unsigned long long)(r - l + 1)); }

#define CHECK(cond) do { if (!(cond)) { \
        cerr << "FAIL " << __FILE__ << ":" << __LINE__ << ": " #cond "\n"; exit(1); } } while (0)
#define CHECK_EQ(a, b) do { auto _a = (a); auto _b = (b); if (!(_a == _b)) { \
        cerr << "FAIL " << __FILE__ << ":" << __LINE__ << ": " #a " != " #b "\n"; \
        cerr << "  got: " << _a << "\n  exp: " << _b << "\n"; exit(1); } } while (0)

template<typename T> ostream& operator<<(ostream& os, const vector<T>& v) {
        for (auto& x : v) os << x << ' ';
        return os;
}

template<typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) {
        return os << '(' << p.first << ',' << p.second << ')';
}
template<typename T> ostream& operator<<(ostream& os, const set<T>& s) {
        for (auto& x : s) os << x << ' ';
        return os;
}
