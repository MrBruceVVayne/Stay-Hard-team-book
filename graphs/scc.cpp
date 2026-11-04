#pragma once
#include "tmp.cpp"

// Тарьян: comp[v] -- номер КСС, номера идут в обратном топологическом порядке
// (для ребра u -> v в разные КСС comp[u] > comp[v]). Рекурсия: при глубине ~1e6 нужен ulimit -s.
struct SCC {
        int n, timer = 0, cnt = 0;
        ve<vi> g;
        vi tin, low, comp, stk;
        vb on;

        SCC(int _n) : n(_n), g(_n), tin(_n, -1), low(_n), comp(_n, -1), on(_n, false) {}

        void addEdge(int a, int b) {
                g[a].push_back(b);
        }

        void dfs(int v) {
                tin[v] = low[v] = timer++;
                stk.push_back(v);
                on[v] = true;
                for (int u : g[v]) {
                        if (tin[u] == -1) {
                                dfs(u);
                                low[v] = min(low[v], low[u]);
                        } else if (on[u]) {
                                low[v] = min(low[v], tin[u]);
                        }
                }
                if (low[v] == tin[v]) {
                        while (true) {
                                int u = stk.back();
                                stk.pop_back();
                                on[u] = false;
                                comp[u] = cnt;
                                if (u == v) break;
                        }
                        cnt++;
                }
        }

        int run() {
                for (int v = 0; v < n; v++) if (tin[v] == -1) dfs(v);
                return cnt;
        }
};

// 2-SAT: переменная i -- литералы 2i (истина) и 2i + 1 (ложь); lit ^ 1 -- отрицание.
struct TwoSat {
        int n;
        SCC s;
        vb val;

        TwoSat(int _n) : n(_n), s(2 * _n) {}

        void addClause(int a, int b) {  // (a или b)
                s.addEdge(a ^ 1, b);
                s.addEdge(b ^ 1, a);
        }

        bool solve() {
                s.run();
                val.assign(n, false);
                for (int i = 0; i < n; i++) {
                        if (s.comp[2 * i] == s.comp[2 * i + 1]) return false;
                        val[i] = s.comp[2 * i] < s.comp[2 * i + 1];
                }
                return true;
        }
};
