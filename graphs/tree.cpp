#pragma once
#include "tmp.cpp"

// HLD: корень 0. pos[v] -- позиция в массиве, поддерево v = [pos[v], pos[v] + sz[v]).
// path(u, v) -- отрезки позиций [l, r] (вершинные значения), lca за O(log n).
struct HLD {
        int n;
        ve<vi> g;
        vi par, dep, sz, heavy, head, pos;

        HLD(int _n) : n(_n), g(_n), par(_n, -1), dep(_n), sz(_n), heavy(_n, -1), head(_n), pos(_n) {}

        void addEdge(int a, int b) {
                g[a].push_back(b);
                g[b].push_back(a);
        }

        void dfs(int v) {
                sz[v] = 1;
                for (int u : g[v]) {
                        if (u == par[v]) continue;
                        par[u] = v;
                        dep[u] = dep[v] + 1;
                        dfs(u);
                        sz[v] += sz[u];
                        if (heavy[v] == -1 || sz[u] > sz[heavy[v]]) heavy[v] = u;
                }
        }

        void build(int root = 0) {
                dfs(root);
                int cur = 0;
                function<void(int, int)> go = [&](int v, int h) {
                        head[v] = h;
                        pos[v] = cur++;
                        if (heavy[v] != -1) go(heavy[v], h);
                        for (int u : g[v]) if (u != par[v] && u != heavy[v]) go(u, u);
                };
                go(root, root);
        }

        int lca(int a, int b) {
                while (head[a] != head[b]) {
                        if (dep[head[a]] < dep[head[b]]) swap(a, b);
                        a = par[head[a]];
                }
                return dep[a] < dep[b] ? a : b;
        }

        ve<pii> path(int a, int b) {  // отрезки позиций, покрывающие путь a..b
                ve<pii> res;
                while (head[a] != head[b]) {
                        if (dep[head[a]] < dep[head[b]]) swap(a, b);
                        res.push_back({pos[head[a]], pos[a]});
                        a = par[head[a]];
                }
                res.push_back({min(pos[a], pos[b]), max(pos[a], pos[b])});
                return res;
        }
};

// Центроидная декомпозиция: cpar -- родитель в дереве центроидов, clvl -- глубина, O(n log n)
struct Centroid {
        int n;
        ve<vi> g;
        vi cpar, clvl, sz;
        vb removed;

        Centroid(int _n) : n(_n), g(_n), cpar(_n, -1), clvl(_n, 0), sz(_n), removed(_n, false) {}

        void addEdge(int a, int b) {
                g[a].push_back(b);
                g[b].push_back(a);
        }

        int calc(int v, int p) {
                sz[v] = 1;
                for (int u : g[v]) if (u != p && !removed[u]) sz[v] += calc(u, v);
                return sz[v];
        }

        int findC(int v, int p, int tot) {
                for (int u : g[v]) if (u != p && !removed[u] && sz[u] * 2 > tot) return findC(u, v, tot);
                return v;
        }

        void build(int v = 0, int p = -1, int lvl = 0) {
                int tot = calc(v, -1);
                int c = findC(v, -1, tot);
                cpar[c] = p;
                clvl[c] = lvl;
                removed[c] = true;
                for (int u : g[c]) if (!removed[u]) build(u, c, lvl + 1);
        }
};
