#include "tmp.cpp"

// Разреженная таблица (min), запрос на [l, r), r > l, O(1).
struct SparseTable {
        ve<vi> spt;

        SparseTable() {}
        SparseTable(const vi &a) {
                int n = a.size(), logn = 32 - __builtin_clz(max(n, 1));
                spt.assign(logn, vi(n, INF));
                spt[0] = a;
                for (int l = 0; l + 1 < logn; l++) {
                        for (int i = 0; i + (2 << l) <= n; i++) {
                                spt[l + 1][i] = min(spt[l][i], spt[l][i + (1 << l)]);
                        }
                }
        }

        int get(int l, int r) {
                int k = 31 - __builtin_clz(r - l);
                return min(spt[k][l], spt[k][r - (1 << k)]);
        }
};
