#pragma once
#include "tmp.cpp"

// Диниц, O(V^2 E) в общем случае, O(E sqrt V) на единичных сетях. Ребро e и обратное e ^ 1.
struct Dinic {
        struct Edge {
                int to;
                ll cap;
        };

        int n;
        ve<Edge> e;
        ve<vi> g;
        vi lvl, ptr;

        Dinic() {}
        Dinic(int _n) : n(_n), g(_n) {}

        int addEdge(int a, int b, ll c, ll rc = 0) {  // rc = c для неориентированного
                g[a].push_back(e.size());
                e.push_back({b, c});
                g[b].push_back(e.size());
                e.push_back({a, rc});
                return (int)e.size() - 2;
        }

        bool bfs(int s, int t) {
                lvl.assign(n, -1);
                lvl[s] = 0;
                queue<int> q;
                q.push(s);
                while (!q.empty()) {
                        int v = q.front();
                        q.pop();
                        for (int id : g[v]) {
                                if (e[id].cap > 0 && lvl[e[id].to] == -1) {
                                        lvl[e[id].to] = lvl[v] + 1;
                                        q.push(e[id].to);
                                }
                        }
                }
                return lvl[t] != -1;
        }

        ll dfs(int v, int t, ll f) {
                if (v == t || !f) return f;
                for (int &i = ptr[v]; i < (int)g[v].size(); i++) {
                        int id = g[v][i], u = e[id].to;
                        if (lvl[u] != lvl[v] + 1 || e[id].cap <= 0) continue;
                        ll d = dfs(u, t, min(f, e[id].cap));
                        if (d) {
                                e[id].cap -= d;
                                e[id ^ 1].cap += d;
                                return d;
                        }
                }
                return 0;
        }

        ll maxflow(int s, int t) {
                ll flow = 0;
                while (bfs(s, t)) {
                        ptr.assign(n, 0);
                        while (ll f = dfs(s, t, INFLL)) flow += f;
                }
                return flow;
        }

        bool inS(int v) {  // после maxflow: v достижима из s в остаточной сети (мин. разрез)
                return lvl[v] != -1;
        }
};

// Дерево Гомори-Ху (Гусфилд): ребро (i, par[i]) веса w[i]. Мин. разрез (u, v) = min на пути.
// edges: неориентированные рёбра (a, b, c).
pair<vi, vll> gomoryHu(int n, const ve<array<ll, 3>> &edges) {
        Dinic base(n);
        for (auto [a, b, c] : edges) base.addEdge(a, b, c, c);
        vi par(n, 0);
        vll w(n, 0);
        for (int i = 1; i < n; i++) {
                Dinic d = base;
                w[i] = d.maxflow(i, par[i]);
                for (int j = i + 1; j < n; j++) {
                        if (d.inS(j) && par[j] == par[i]) par[j] = i;
                }
        }
        return {par, w};
}

// Глобальный минимальный разрез (Штор-Вагнер), O(n^3), матрица весов w. Возвращает вес.
ll stoerWagner(ve<vll> w) {
        int n = w.size();
        ll best = INFLL;
        vi vs(n);
        iota(all(vs), 0);
        while (vs.size() > 1) {
                int k = vs.size();
                vll key(k, 0);
                vb used(k, false);
                int prev = -1, last = -1;
                for (int it = 0; it < k; it++) {
                        int sel = -1;
                        for (int i = 0; i < k; i++) {
                                if (!used[i] && (sel == -1 || key[i] > key[sel])) sel = i;
                        }
                        used[sel] = true;
                        prev = last;
                        last = sel;
                        if (it == k - 1) best = min(best, key[sel]);
                        else for (int i = 0; i < k; i++) if (!used[i]) key[i] += w[vs[sel]][vs[i]];
                }
                // сливаем последнюю с предпоследней
                int a = vs[prev], b = vs[last];
                for (int i = 0; i < n; i++) w[a][i] += w[b][i], w[i][a] = w[a][i];
                w[a][a] = 0;
                vs.erase(vs.begin() + last);
        }
        return best;
}
