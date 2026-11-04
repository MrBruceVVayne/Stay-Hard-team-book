#pragma once
#include "tmp.cpp"

// Дейкстра, O(E log V). g[v] = (u, w), w >= 0
vll dijkstra(const ve<ve<pair<int, ll>>> &g, int s) {
        vll d(g.size(), INFLL);
        priority_queue<pll, ve<pll>, greater<pll>> pq;
        d[s] = 0;
        pq.push({0, s});
        while (!pq.empty()) {
                auto [dv, v] = pq.top();
                pq.pop();
                if (dv != d[v]) continue;
                for (auto [u, w] : g[v]) {
                        if (d[u] > dv + w) {
                                d[u] = dv + w;
                                pq.push({d[u], u});
                        }
                }
        }
        return d;
}

// Форд-Беллман: false, если из s достижим отрицательный цикл. edges (a, b, w)
bool bellmanFord(int n, const ve<array<ll, 3>> &edges, int s, vll &d) {
        d.assign(n, INFLL);
        d[s] = 0;
        for (int it = 0; it < n; it++) {
                bool ch = false;
                for (auto [a, b, w] : edges) {
                        if (d[a] != INFLL && d[b] > d[a] + w) {
                                d[b] = d[a] + w;
                                ch = true;
                        }
                }
                if (!ch) return true;
        }
        return false;
}

void floyd(ve<vll> &d) {  // d[i][j] = INFLL, если ребра нет; d[i][i] = 0
        int n = d.size();
        for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
                if (d[i][k] < INFLL && d[k][j] < INFLL) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        }
}

// Эйлеров путь/цикл (Хирхольцер). edges (a, b); возвращает вершины пути (m + 1 штук) или пусто.
vi eulerPath(int n, const ve<pii> &edges, bool directed) {
        int m = edges.size();
        ve<vi> g(n);
        vi deg(n, 0), in(n, 0);
        for (int i = 0; i < m; i++) {
                auto [a, b] = edges[i];
                g[a].push_back(i);
                if (directed) in[b]++;
                else g[b].push_back(i);
                deg[a]++;
                if (!directed) deg[b]++;
        }
        int start = -1, odd = 0, cs = 0, ce = 0;
        for (int v = 0; v < n; v++) {
                if (directed) {
                        int d = deg[v] - in[v];
                        if (d == 1) cs++, start = v;
                        else if (d == -1) ce++;
                        else if (d != 0) return {};
                } else if (deg[v] & 1) {
                        odd++;
                        start = v;
                }
        }
        if (!directed && odd > 2) return {};
        if (directed && (cs > 1 || ce > 1 || cs != ce)) return {};
        if (start == -1) for (int v = 0; v < n; v++) if (!g[v].empty()) { start = v; break; }
        if (start == -1) return {0};
        vb used(m, false);
        vi ptr(n, 0), st = {start}, res;
        while (!st.empty()) {
                int v = st.back();
                while (ptr[v] < (int)g[v].size() && used[g[v][ptr[v]]]) ptr[v]++;
                if (ptr[v] == (int)g[v].size()) {
                        res.push_back(v);
                        st.pop_back();
                } else {
                        int id = g[v][ptr[v]];
                        used[id] = true;
                        st.push_back(edges[id].first == v ? edges[id].second : edges[id].first);
                }
        }
        if ((int)res.size() != m + 1) return {};
        reverse(all(res));
        return res;
}

// Минимальный остов ориентированного графа с корнем root (Чу-Лю/Эдмондс), O(VE).
// edges (u, v, w): u -> v. Возвращает вес или -1.
ll dirMST(int n, int root, ve<array<ll, 3>> edges) {
        ll res = 0;
        while (true) {
                vll in(n, INFLL);
                vi pre(n, -1);
                for (auto [u, v, w] : edges) if (u != v && w < in[v]) in[v] = w, pre[v] = u;
                for (int i = 0; i < n; i++) if (i != root && in[i] == INFLL) return -1;
                int cnt = 0;
                vi id(n, -1), vis(n, -1);
                in[root] = 0;
                for (int v = 0; v < n; v++) {
                        res += in[v];
                        int u = v;
                        while (vis[u] != v && id[u] == -1 && u != root) vis[u] = v, u = pre[u];
                        if (u != root && id[u] == -1) {
                                for (int x = pre[u]; x != u; x = pre[x]) id[x] = cnt;
                                id[u] = cnt++;
                        }
                }
                if (!cnt) break;
                for (int v = 0; v < n; v++) if (id[v] == -1) id[v] = cnt++;
                for (auto &e : edges) {
                        ll old = e[1];
                        e[0] = id[e[0]], e[1] = id[e[1]];
                        if (e[0] != e[1]) e[2] -= in[old];
                }
                n = cnt;
                root = id[root];
        }
        return res;
}
