#pragma once
#include "dataStructures/dsu.cpp"

// Оффлайн динамическая связность: рёбра живут на отрезке времени [l, r). Дерево отрезков по
// времени + СНМ с откатами, O((n + m) log T log n). run() -- число компонент в каждый момент.
struct OfflineConn {
        int n, T, comps;
        ve<ve<pii>> seg;
        DSU d;
        vi ans;

        OfflineConn(int _n, int _T) : n(_n), T(_T), comps(_n), seg(4 * _T + 4), d(_n), ans(_T) {}

        void addEdge(int l, int r, int u, int v, int node, int tl, int tr) {
                if (r <= tl || tr <= l) return;
                if (l <= tl && tr <= r) {
                        seg[node].push_back({u, v});
                        return;
                }
                int mid = (tl + tr) / 2;
                addEdge(l, r, u, v, 2 * node, tl, mid);
                addEdge(l, r, u, v, 2 * node + 1, mid, tr);
        }

        void addEdge(int l, int r, int u, int v) {
                addEdge(l, r, u, v, 1, 0, T);
        }

        void rec(int node, int tl, int tr) {
                int snap = d.snapshot(), cc = comps;
                for (auto [a, b] : seg[node]) if (d.unite(a, b)) comps--;
                if (tr - tl == 1) ans[tl] = comps;
                else {
                        int mid = (tl + tr) / 2;
                        rec(2 * node, tl, mid);
                        rec(2 * node + 1, mid, tr);
                }
                d.rollback(snap);
                comps = cc;
        }

        vi run() {
                rec(1, 0, T);
                return ans;
        }
};
