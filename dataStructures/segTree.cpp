#pragma once
#include "tmp.cpp"

// Дерево отрезков с ленивым прибавлением. Отрезки [l, r). Хранит сумму и максимум.
struct SegTree {
        struct Node {
                ll sum = 0, mx = -INFLL, add = 0;
        };

        int maxn;
        ve<Node> st;

        SegTree() {}
        SegTree(const ve<ll> &a) : maxn(a.size()), st(4 * a.size() + 4) {
                build(a, 1, 0, maxn);
        }

        void pull(int v) {
                st[v].sum = st[2 * v].sum + st[2 * v + 1].sum;
                st[v].mx = max(st[2 * v].mx, st[2 * v + 1].mx);
        }

        void apply(int v, int len, ll x) {
                st[v].sum += x * len;
                st[v].mx += x;
                st[v].add += x;
        }

        void push(int v, int l, int r) {
                if (!st[v].add) return;
                int mid = (l + r) / 2;
                apply(2 * v, mid - l, st[v].add);
                apply(2 * v + 1, r - mid, st[v].add);
                st[v].add = 0;
        }

        void build(const ve<ll> &a, int v, int l, int r) {
                if (r - l == 1) {
                        st[v].sum = st[v].mx = a[l];
                        return;
                }
                int mid = (l + r) / 2;
                build(a, 2 * v, l, mid);
                build(a, 2 * v + 1, mid, r);
                pull(v);
        }

        void update(int ql, int qr, ll x, int v, int l, int r) {
                if (qr <= l || r <= ql) return;
                if (ql <= l && r <= qr) {
                        apply(v, r - l, x);
                        return;
                }
                push(v, l, r);
                int mid = (l + r) / 2;
                update(ql, qr, x, 2 * v, l, mid);
                update(ql, qr, x, 2 * v + 1, mid, r);
                pull(v);
        }

        void update(int ql, int qr, ll x) {
                update(ql, qr, x, 1, 0, maxn);
        }

        ll getSum(int ql, int qr, int v, int l, int r) {
                if (qr <= l || r <= ql) return 0;
                if (ql <= l && r <= qr) return st[v].sum;
                push(v, l, r);
                int mid = (l + r) / 2;
                return getSum(ql, qr, 2 * v, l, mid) + getSum(ql, qr, 2 * v + 1, mid, r);
        }

        ll getSum(int ql, int qr) {
                return getSum(ql, qr, 1, 0, maxn);
        }

        ll getMax(int ql, int qr, int v, int l, int r) {
                if (qr <= l || r <= ql) return -INFLL;
                if (ql <= l && r <= qr) return st[v].mx;
                push(v, l, r);
                int mid = (l + r) / 2;
                return max(getMax(ql, qr, 2 * v, l, mid), getMax(ql, qr, 2 * v + 1, mid, r));
        }

        ll getMax(int ql, int qr) {
                return getMax(ql, qr, 1, 0, maxn);
        }

        // первый i >= ql с a[i] >= x, иначе -1
        int firstGe(int ql, ll x, int v, int l, int r) {
                if (r <= ql || st[v].mx < x) return -1;
                if (r - l == 1) return l;
                push(v, l, r);
                int mid = (l + r) / 2;
                int res = firstGe(ql, x, 2 * v, l, mid);
                return (res != -1) ? res : firstGe(ql, x, 2 * v + 1, mid, r);
        }

        int firstGe(int ql, ll x) {
                return firstGe(ql, x, 1, 0, maxn);
        }
};
