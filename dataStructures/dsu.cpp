#include "tmp.cpp"

// СНМ с откатами (без сжатия путей, поэтому O(log n) на операцию).
// Для обычного СНМ: убрать hist и сделать p[v] = find(p[v]).
struct DSU {
        vi p, sz;
        ve<pii> hist;  // (присоединённый корень, его новый родитель)

        DSU() {}
        DSU(int n) : p(n), sz(n, 1) {
                iota(all(p), 0);
        }

        int find(int v) {
                return p[v] == v ? v : find(p[v]);
        }

        bool unite(int a, int b) {
                a = find(a);
                b = find(b);
                if (a == b) return false;
                if (sz[a] < sz[b]) swap(a, b);
                p[b] = a;
                sz[a] += sz[b];
                hist.push_back({b, a});
                return true;
        }

        int snapshot() {
                return hist.size();
        }

        void rollback(int snap) {
                while ((int)hist.size() > snap) {
                        auto [b, a] = hist.back();
                        hist.pop_back();
                        p[b] = b;
                        sz[a] -= sz[b];
                }
        }
};
