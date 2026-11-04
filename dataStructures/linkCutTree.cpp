#include "tmp.cpp"

// Link-Cut Tree: вершины 1..n. Лес с корнями, makeRoot/link/cut/connected, сумма на пути.
struct LinkCut {
        struct Node {
                int ch[2] = {0, 0}, p = 0;
                bool rev = false;
                ll val = 0, sum = 0;
        };

        ve<Node> t;

        LinkCut(int n) : t(n + 1) {}

        bool isRoot(int x) {
                int p = t[x].p;
                return !p || (t[p].ch[0] != x && t[p].ch[1] != x);
        }

        void pull(int x) {
                t[x].sum = t[x].val + t[t[x].ch[0]].sum + t[t[x].ch[1]].sum;
        }

        void flip(int x) {
                if (!x) return;
                swap(t[x].ch[0], t[x].ch[1]);
                t[x].rev ^= 1;
        }

        void push(int x) {
                if (!t[x].rev) return;
                flip(t[x].ch[0]);
                flip(t[x].ch[1]);
                t[x].rev = false;
        }

        void rotate(int x) {
                int p = t[x].p, g = t[p].p, d = (t[p].ch[1] == x);
                if (!isRoot(p)) t[g].ch[t[g].ch[1] == p] = x;
                t[x].p = g;
                t[p].ch[d] = t[x].ch[d ^ 1];
                if (t[x].ch[d ^ 1]) t[t[x].ch[d ^ 1]].p = p;
                t[x].ch[d ^ 1] = p;
                t[p].p = x;
                pull(p);
        }

        void splay(int x) {
                vi stk;
                for (int y = x;; y = t[y].p) {
                        stk.push_back(y);
                        if (isRoot(y)) break;
                }
                while (!stk.empty()) {
                        push(stk.back());
                        stk.pop_back();
                }
                while (!isRoot(x)) {
                        int p = t[x].p;
                        if (!isRoot(p)) {
                                int g = t[p].p;
                                rotate(((t[g].ch[1] == p) == (t[p].ch[1] == x)) ? p : x);
                        }
                        rotate(x);
                }
                pull(x);
        }

        void access(int x) {
                int last = 0;
                for (int y = x; y; y = t[y].p) {
                        splay(y);
                        t[y].ch[1] = last;
                        pull(y);
                        last = y;
                }
                splay(x);
        }

        void makeRoot(int x) {
                access(x);
                flip(x);
        }

        int findRoot(int x) {
                access(x);
                while (true) {
                        push(x);
                        if (!t[x].ch[0]) break;
                        x = t[x].ch[0];
                }
                splay(x);
                return x;
        }

        bool connected(int x, int y) {
                return findRoot(x) == findRoot(y);
        }

        bool link(int x, int y) {
                makeRoot(x);
                if (findRoot(y) == x) return false;
                t[x].p = y;
                return true;
        }

        bool cut(int x, int y) {  // удаляет ребро x-y, если оно есть
                makeRoot(x);
                access(y);
                if (t[y].ch[0] != x || t[x].ch[0] || t[x].ch[1]) return false;
                t[y].ch[0] = 0;
                t[x].p = 0;
                pull(y);
                return true;
        }

        void set(int x, ll v) {
                access(x);
                t[x].val = v;
                pull(x);
        }

        ll pathSum(int x, int y) {  // x, y в одном дереве
                makeRoot(x);
                access(y);
                return t[y].sum;
        }
};
