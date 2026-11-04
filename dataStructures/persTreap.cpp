#include "tmp.cpp"

mt19937 prng(7);

// Персистентное неявное декартово дерево (без ленивых операций). Версия = корень (int).
// Каждая операция возвращает новый корень, старые версии остаются валидными.
// Память: O(log n) новых вершин на операцию.
struct PersTreap {
        struct Node {
                int l = 0, r = 0, sz = 1, y = 0;
                ll x = 0, sum = 0;
        };

        ve<Node> t;

        PersTreap() : t(1) {
                t[0].sz = 0;
        }

        int newNode(ll x) {
                Node v;
                v.x = v.sum = x;
                v.y = prng();
                t.push_back(v);
                return (int)t.size() - 1;
        }

        int copy(int v) {
                Node c = t[v];
                t.push_back(c);
                return (int)t.size() - 1;
        }

        void pull(int v) {
                int l = t[v].l, r = t[v].r;
                t[v].sz = 1 + t[l].sz + t[r].sz;
                t[v].sum = t[v].x + t[l].sum + t[r].sum;
        }

        pii split(int v, int k) {
                if (!v) return {0, 0};
                int c = copy(v);
                if (t[t[v].l].sz >= k) {
                        auto [a, b] = split(t[v].l, k);
                        t[c].l = b;
                        pull(c);
                        return {a, c};
                }
                auto [a, b] = split(t[v].r, k - t[t[v].l].sz - 1);
                t[c].r = a;
                pull(c);
                return {c, b};
        }

        int merge(int a, int b) {
                if (!a || !b) return a ? a : b;
                int c;
                if (t[a].y > t[b].y) {
                        c = copy(a);
                        int r = merge(t[a].r, b);
                        t[c].r = r;
                } else {
                        c = copy(b);
                        int l = merge(a, t[b].l);
                        t[c].l = l;
                }
                pull(c);
                return c;
        }

        int build(const ve<ll> &a) {
                int root = 0;
                for (ll x : a) root = merge(root, newNode(x));
                return root;
        }

        int insert(int root, int pos, ll x) {
                auto [a, b] = split(root, pos);
                return merge(merge(a, newNode(x)), b);
        }

        int erase(int root, int pos) {
                auto [a, b] = split(root, pos);
                auto [c, d] = split(b, 1);
                return merge(a, d);
        }

        ll sum(int root, int l, int r) {  // [l, r)
                auto [a, b] = split(root, l);
                auto [c, d] = split(b, r - l);
                return t[c].sum;
        }

        int size(int root) { return t[root].sz; }
};
