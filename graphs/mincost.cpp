#pragma once
#include "tmp.cpp"

// Мин-кост макс-флоу (SPFA), допускает отрицательные стоимости без отрицательных циклов.
struct MCMF {
        struct Edge {
                int to;
                ll cap, cost;
        };

        int n;
        ve<Edge> e;
        ve<vi> g;

        MCMF(int _n) : n(_n), g(_n) {}

        void addEdge(int a, int b, ll cap, ll cost) {
                g[a].push_back(e.size());
                e.push_back({b, cap, cost});
                g[b].push_back(e.size());
                e.push_back({a, 0, -cost});
        }

        // {поток, стоимость}; пускает не больше maxf
        pll run(int s, int t, ll maxf = INFLL) {
                ll flow = 0, cost = 0;
                while (flow < maxf) {
                        vll dist(n, INFLL);
                        vi par(n, -1);
                        vb inq(n, false);
                        deque<int> q;
                        dist[s] = 0;
                        q.push_back(s);
                        while (!q.empty()) {
                                int v = q.front();
                                q.pop_front();
                                inq[v] = false;
                                for (int id : g[v]) {
                                        if (e[id].cap > 0 && dist[e[id].to] > dist[v] + e[id].cost) {
                                                dist[e[id].to] = dist[v] + e[id].cost;
                                                par[e[id].to] = id;
                                                if (!inq[e[id].to]) {
                                                        inq[e[id].to] = true;
                                                        q.push_back(e[id].to);
                                                }
                                        }
                                }
                        }
                        if (dist[t] == INFLL) break;
                        ll f = maxf - flow;
                        for (int v = t; v != s; v = e[par[v] ^ 1].to) f = min(f, e[par[v]].cap);
                        for (int v = t; v != s; v = e[par[v] ^ 1].to) {
                                e[par[v]].cap -= f;
                                e[par[v] ^ 1].cap += f;
                        }
                        flow += f;
                        cost += f * dist[t];
                }
                return {flow, cost};
        }
};
