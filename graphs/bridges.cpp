#pragma once
#include "tmp.cpp"

// Мосты, точки сочленения и вершинные компоненты двусвязности. Мультиребра допустимы.
struct Bridges {
        int n, timer = 0;
        ve<ve<pii>> g;  // (куда, номер ребра)
        vi tin, low, stk;
        vb isCut, isBridge;
        ve<vi> bcc;  // вершинные компоненты (каждая -- список вершин)

        Bridges(int _n, const ve<pii> &edges) : n(_n), g(_n), tin(_n, -1), low(_n), isCut(_n, false),
                                                 isBridge(edges.size(), false) {
                for (int i = 0; i < (int)edges.size(); i++) {
                        g[edges[i].first].push_back({edges[i].second, i});
                        g[edges[i].second].push_back({edges[i].first, i});
                }
                for (int v = 0; v < n; v++) if (tin[v] == -1) {
                        dfs(v, -1, true);
                }
        }

        void dfs(int v, int pe, bool root) {
                tin[v] = low[v] = timer++;
                stk.push_back(v);
                int ch = 0;
                for (auto [u, id] : g[v]) {
                        if (id == pe) continue;
                        if (tin[u] != -1) {
                                low[v] = min(low[v], tin[u]);
                                continue;
                        }
                        dfs(u, id, false);
                        ch++;
                        low[v] = min(low[v], low[u]);
                        if (low[u] > tin[v]) isBridge[id] = true;
                        if (low[u] >= tin[v]) {
                                if (!root) isCut[v] = true;
                                bcc.push_back({v});
                                while (true) {
                                        int w = stk.back();
                                        stk.pop_back();
                                        bcc.back().push_back(w);
                                        if (w == u) break;
                                }
                        }
                }
                if (root && ch > 1) isCut[v] = true;
                if (root && ch == 0) bcc.push_back({v});
        }
};
