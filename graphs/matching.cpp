#pragma once
#include "tmp.cpp"

// Хопкрофт-Карп: доли [0, n) и [0, m), O(E sqrt V). После solve есть вершинное покрытие (Кёниг).
struct HopcroftKarp {
        int n, m;
        ve<vi> g;
        vi ml, mr, dist;

        HopcroftKarp(int _n, int _m) : n(_n), m(_m), g(_n), ml(_n, -1), mr(_m, -1) {}

        void addEdge(int l, int r) {
                g[l].push_back(r);
        }

        bool bfs() {
                dist.assign(n, -1);
                queue<int> q;
                for (int v = 0; v < n; v++) if (ml[v] == -1) dist[v] = 0, q.push(v);
                bool found = false;
                while (!q.empty()) {
                        int v = q.front();
                        q.pop();
                        for (int u : g[v]) {
                                int w = mr[u];
                                if (w == -1) found = true;
                                else if (dist[w] == -1) dist[w] = dist[v] + 1, q.push(w);
                        }
                }
                return found;
        }

        bool dfs(int v) {
                for (int u : g[v]) {
                        int w = mr[u];
                        if (w == -1 || (dist[w] == dist[v] + 1 && dfs(w))) {
                                ml[v] = u;
                                mr[u] = v;
                                return true;
                        }
                }
                dist[v] = -1;
                return false;
        }

        int solve() {
                int res = 0;
                while (bfs()) {
                        for (int v = 0; v < n; v++) if (ml[v] == -1 && dfs(v)) res++;
                }
                return res;
        }

        // min вершинное покрытие: (левые, правые); независимое множество -- дополнение
        pair<vi, vi> cover() {
                vb visL(n, false), visR(m, false);
                vi st;
                for (int v = 0; v < n; v++) if (ml[v] == -1) visL[v] = true, st.push_back(v);
                while (!st.empty()) {
                        int v = st.back();
                        st.pop_back();
                        for (int u : g[v]) {
                                if (visR[u]) continue;
                                visR[u] = true;
                                int w = mr[u];
                                if (w != -1 && !visL[w]) visL[w] = true, st.push_back(w);
                        }
                }
                vi L, R;
                for (int v = 0; v < n; v++) if (!visL[v]) L.push_back(v);
                for (int u = 0; u < m; u++) if (visR[u]) R.push_back(u);
                return {L, R};
        }
};

// Паросочетание в общем графе (Эдмондс, стягивание цветков), O(V^3)
struct Blossom {
        int n;
        ve<vi> g;
        vi match, p, base, q;
        vb used, blossom;

        Blossom(int _n) : n(_n), g(_n), match(_n, -1) {}

        void addEdge(int a, int b) {
                g[a].push_back(b);
                g[b].push_back(a);
        }

        int lca(int a, int b) {
                vb u(n, false);
                while (true) {
                        a = base[a];
                        u[a] = true;
                        if (match[a] == -1) break;
                        a = p[match[a]];
                }
                while (true) {
                        b = base[b];
                        if (u[b]) return b;
                        b = p[match[b]];
                }
        }

        void markPath(int v, int b, int child) {
                while (base[v] != b) {
                        blossom[base[v]] = blossom[base[match[v]]] = true;
                        p[v] = child;
                        child = match[v];
                        v = p[match[v]];
                }
        }

        int findPath(int root) {
                used.assign(n, false);
                p.assign(n, -1);
                base.resize(n);
                iota(all(base), 0);
                used[root] = true;
                q.assign(1, root);
                for (size_t qh = 0; qh < q.size(); qh++) {
                        int v = q[qh];
                        for (int to : g[v]) {
                                if (base[v] == base[to] || match[v] == to) continue;
                                if (to == root || (match[to] != -1 && p[match[to]] != -1)) {
                                        int cur = lca(v, to);
                                        blossom.assign(n, false);
                                        markPath(v, cur, to);
                                        markPath(to, cur, v);
                                        for (int i = 0; i < n; i++) {
                                                if (!blossom[base[i]]) continue;
                                                base[i] = cur;
                                                if (!used[i]) used[i] = true, q.push_back(i);
                                        }
                                } else if (p[to] == -1) {
                                        p[to] = v;
                                        if (match[to] == -1) return to;
                                        used[match[to]] = true;
                                        q.push_back(match[to]);
                                }
                        }
                }
                return -1;
        }

        int solve() {
                int res = 0;
                for (int i = 0; i < n; i++) {
                        if (match[i] != -1) continue;
                        int v = findPath(i);
                        if (v == -1) continue;
                        res++;
                        while (v != -1) {
                                int pv = p[v], ppv = match[pv];
                                match[v] = pv;
                                match[pv] = v;
                                v = ppv;
                        }
                }
                return res;
        }
};

// Венгерский алгоритм: a[1..n][1..m], n <= m, минимизирует стоимость. O(n^2 m)
// Возвращает {стоимость, ans[i] = столбец строки i}
pair<ll, vi> hungarian(const ve<vll> &a, int n, int m) {
        vll u(n + 1), v(m + 1);
        vi p(m + 1), way(m + 1);
        for (int i = 1; i <= n; i++) {
                p[0] = i;
                int j0 = 0;
                vll minv(m + 1, INFLL);
                vb used(m + 1, false);
                do {
                        used[j0] = true;
                        int i0 = p[j0], j1 = 0;
                        ll delta = INFLL;
                        for (int j = 1; j <= m; j++) {
                                if (used[j]) continue;
                                ll cur = a[i0][j] - u[i0] - v[j];
                                if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                                if (minv[j] < delta) delta = minv[j], j1 = j;
                        }
                        for (int j = 0; j <= m; j++) {
                                if (used[j]) u[p[j]] += delta, v[j] -= delta;
                                else minv[j] -= delta;
                        }
                        j0 = j1;
                } while (p[j0] != 0);
                do {
                        int j1 = way[j0];
                        p[j0] = p[j1];
                        j0 = j1;
                } while (j0);
        }
        vi ans(n + 1);
        for (int j = 1; j <= m; j++) if (p[j]) ans[p[j]] = j;
        return {-v[0], ans};
}
