#include "tmp.cpp"

mt19937 rng(1543);

// Неявное декартово дерево (индексы вместо указателей): вставка/удаление по позиции,
// разворот и сумма на отрезке [l, r). t[0] -- null-вершина.
struct Treap {
        struct Node {
                int l = 0, r = 0, sz = 1, y = 0;
                ll x = 0, sum = 0;
                bool rev = false;
        };

        ve<Node> t;
        int root = 0;

        Treap() : t(1) {
                t[0].sz = 0;
        }

        int newNode(ll x) {
                Node v;
                v.x = v.sum = x;
                v.y = rng();
                t.push_back(v);
                return (int)t.size() - 1;
        }

        void pull(int v) {
                int l = t[v].l, r = t[v].r;
                t[v].sz = 1 + t[l].sz + t[r].sz;
                t[v].sum = t[v].x + t[l].sum + t[r].sum;
        }

        void applyRev(int v) {
                if (!v) return;
                t[v].rev ^= 1;
                swap(t[v].l, t[v].r);
        }

        void push(int v) {
                if (!t[v].rev) return;
                applyRev(t[v].l);
                applyRev(t[v].r);
                t[v].rev = false;
        }

        pii split(int v, int k) {  // первые k вершин -> левое дерево
                if (!v) return {0, 0};
                push(v);
                if (t[t[v].l].sz >= k) {
                        auto [a, b] = split(t[v].l, k);
                        t[v].l = b;
                        pull(v);
                        return {a, v};
                }
                auto [a, b] = split(t[v].r, k - t[t[v].l].sz - 1);
                t[v].r = a;
                pull(v);
                return {v, b};
        }

        int merge(int a, int b) {
                if (!a || !b) return a ? a : b;
                if (t[a].y > t[b].y) {
                        push(a);
                        int c = merge(t[a].r, b);
                        t[a].r = c;
                        pull(a);
                        return a;
                }
                push(b);
                int c = merge(a, t[b].l);
                t[b].l = c;
                pull(b);
                return b;
        }

        int size() { return t[root].sz; }

        void insert(int pos, ll x) {
                auto [a, b] = split(root, pos);
                root = merge(merge(a, newNode(x)), b);
        }

        void erase(int pos) {
                auto [a, b] = split(root, pos);
                auto [c, d] = split(b, 1);
                root = merge(a, d);
        }

        void reverse(int l, int r) {
                auto [a, b] = split(root, l);
                auto [c, d] = split(b, r - l);
                applyRev(c);
                root = merge(a, merge(c, d));
        }

        ll sum(int l, int r) {
                auto [a, b] = split(root, l);
                auto [c, d] = split(b, r - l);
                ll res = t[c].sum;
                root = merge(a, merge(c, d));
                return res;
        }

        ll get(int pos) {
                return sum(pos, pos + 1);
        }

        void dump(int v, ve<ll> &out) {  // содержимое по порядку
                if (!v) return;
                push(v);
                dump(t[v].l, out);
                out.push_back(t[v].x);
                dump(t[v].r, out);
        }
};
