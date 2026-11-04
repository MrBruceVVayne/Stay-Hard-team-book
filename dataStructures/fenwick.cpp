#include "tmp.cpp"

// Фенвик на [0, n). get(l, r) -- сумма на [l, r). lower_bound -- для неотрицательных
// значений: сколько первых элементов имеют суммарно <= pos.
template<typename T>
struct FenwickTree {
        int maxn;
        ve<T> ft;

        FenwickTree() {}
        FenwickTree(int n) : maxn(n), ft(n) {}
        FenwickTree(const ve<T> &s) : maxn(s.size()), ft(s) {
                for (int i = 0; i < maxn; i++) {
                        int j = i | (i + 1);
                        if (j < maxn) ft[j] += ft[i];
                }
        }

        void update(int i, T x) {
                for (; i < maxn; i |= (i + 1)) ft[i] += x;
        }

        T get(int r) {  // сумма на [0, r]
                T res = 0;
                for (r = min(r, maxn - 1); r >= 0; r = (r & (r + 1)) - 1) res += ft[r];
                return res;
        }

        T get(int ql, int qr) {
                return get(qr - 1) - get(ql - 1);
        }

        int lower_bound(T pos) {
                int k = 0;
                for (int l = 29; l >= 0; l--) {
                        if (k + (1 << l) <= maxn && ft[k + (1 << l) - 1] <= pos) {
                                k += (1 << l);
                                pos -= ft[k - 1];
                        }
                }
                return k;
        }
};
