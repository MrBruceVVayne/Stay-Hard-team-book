#include "tmp.cpp"

// Disjoint Sparse Table: запрос на [l, r) за O(1) для ЛЮБОЙ ассоциативной операции
// (не только идемпотентной). Размер дополняется до степени двойки.
struct DST {
        struct Node {
                int x;
                Node() : x(INF) {}
                Node(int _x) : x(_x) {}
        };

        Node merge(const Node &l, const Node &r) {  // заменить на свою операцию
                return (l.x <= r.x) ? l : r;
        }

        int n, logn;
        ve<ve<Node>> dst;

        DST() {}
        DST(const vi &a) {
                logn = 1;
                while ((1 << logn) < (int)a.size()) logn++;
                n = 1 << logn;
                dst.assign(logn, ve<Node>(n));
                for (int d = 0; d < logn; d++) {
                        int len = 1 << d;
                        for (int mid = len; mid < n + len; mid += 2 * len) {
                                // dst[d][i], i < mid -- сумма [i, mid); i >= mid -- сумма [mid, i]
                                if (mid - 1 < (int)a.size()) dst[d][mid - 1] = Node(a[mid - 1]);
                                for (int i = mid - 2; i >= mid - len; i--) {
                                        dst[d][i] = merge(Node(i < (int)a.size() ? a[i] : INF), dst[d][i + 1]);
                                }
                                if (mid < n) dst[d][mid] = Node(mid < (int)a.size() ? a[mid] : INF);
                                for (int i = mid + 1; i < min(n, mid + len); i++) {
                                        dst[d][i] = merge(dst[d][i - 1], Node(i < (int)a.size() ? a[i] : INF));
                                }
                        }
                }
        }

        Node get(int l, int r) {  // [l, r)
                r--;
                if (l == r) return dst[0][l];
                int d = 31 - __builtin_clz(l ^ r);
                return merge(dst[d][l], dst[d][r]);
        }
};
