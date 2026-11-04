#include "tmp.cpp"

// Снизу-вверх, без рекурсии, работает и для некоммутативной операции. [l, r).
// Пример: Node -- функция x -> a * x + b (mod MOD); merge(l, r) = сначала l, потом r.
struct SegTreeFast {
        struct Node {
                ll a, b;
                Node() : a(1), b(0) {}  // нейтральный элемент
                Node(ll _a, ll _b) : a(_a), b(_b) {}
        };

        Node merge(const Node &l, const Node &r) {
                return Node(l.a * r.a % MOD, (l.b * r.a + r.b) % MOD);
        }

        int maxn;
        ve<Node> st;

        SegTreeFast() {}
        SegTreeFast(int n) : maxn(n), st(2 * n) {}
        SegTreeFast(const ve<Node> &a) : maxn(a.size()), st(2 * a.size()) {
                for (int i = 0; i < maxn; i++) st[maxn + i] = a[i];
                for (int i = maxn - 1; i > 0; i--) st[i] = merge(st[2 * i], st[2 * i + 1]);
        }

        void update(int i, const Node &x) {
                for (st[i += maxn] = x; i > 1; i >>= 1) {
                        st[i >> 1] = merge(st[i & ~1], st[i | 1]);
                }
        }

        Node get(int l, int r) {
                Node resl, resr;
                for (l += maxn, r += maxn; l < r; l >>= 1, r >>= 1) {
                        if (l & 1) resl = merge(resl, st[l++]);
                        if (r & 1) resr = merge(st[--r], resr);
                }
                return merge(resl, resr);
        }
};
