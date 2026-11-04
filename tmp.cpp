#pragma once
#include <bits/stdc++.h>

// #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2")

#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
// #define int long long

using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
template<typename T1, typename T2>
using pr = pair<T1, T2>;
using vi = vector<int>;
using vb = vector<bool>;
using vii = vector<pair<int, int>>;
using vll = vector<long long>;
template<typename T>
using ve = vector<T>;

const long long MOD = 1e9 + 7;
const int INF = 1e9 + 1;
const long long INFLL = 1e18 + 1;

inline void fchmin(int &a, int b) { a = min(a, b); }
inline void fchmax(int &a, int b) { a = max(a, b); }

template<typename T>
inline bool chmin(T &a, T b) {
        if (a > b) {
                a = b;
                return true;
        }
        return false;
}

template<typename T>
inline bool chmax(T &a, T b) {
        if (a < b) {
                a = b;
                return true;
        }
        return false;
}

template <typename... Args>
void dbg(const string& s, const Args&... args) {
        #ifdef DEBUG
        cout << s << ": ";
        ((cout << args << ' '), ...);
        cout << '\n';
        #endif
}

int reps = 1;

void solve() {
        
}

signed main() {
        ios::sync_with_stdio(0);
        cin.tie(0);

        #ifdef LOCAL
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
        #endif
        cin >> reps;

        for (int i = 1; i <= reps; i++) {
                #ifdef LOCAL
                cout << "Test #" << i << " ================\n";
                #endif
                solve();
                #ifdef LOCAL
                cout << "End test ===============\n\n";
                #endif
        }
        #ifdef LOCAL
        cout << "Runtime is: " << clock() * 1.0 / CLOCKS_PER_SEC << endl;
        #endif
}