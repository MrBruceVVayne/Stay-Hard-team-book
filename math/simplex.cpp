#pragma once
#include "tmp.cpp"

// Симплекс: max c^T x, A x <= b, x >= 0. solve возвращает значение (или +-INF если неограничено /
// нет решений: inf / -inf), x -- оптимальная точка. Сложность на практике быстрая, худший случай
// экспоненциальный. Для равенств -- два неравенства. Правило Бланда по (значение, номер).
struct LPSolver {
        int m, n;
        vi N, B;
        ve<ve<ld>> D;
        static constexpr ld e = 1e-9, inf = 1e18;

        LPSolver(const ve<ve<ld>> &A, const ve<ld> &b, const ve<ld> &c)
                : m(b.size()), n(c.size()), N(n + 1), B(m), D(m + 2, ve<ld>(n + 2)) {
                for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) D[i][j] = A[i][j];
                for (int i = 0; i < m; i++) B[i] = n + i, D[i][n] = -1, D[i][n + 1] = b[i];
                for (int j = 0; j < n; j++) N[j] = j, D[m][j] = -c[j];
                N[n] = -1;
                D[m + 1][n] = 1;
        }

        void pivot(int r, int s) {
                ld iv = 1 / D[r][s];
                for (int i = 0; i < m + 2; i++) {
                        if (i == r || abs(D[i][s]) <= e) continue;
                        ld k = D[i][s] * iv;
                        for (int j = 0; j < n + 2; j++) D[i][j] -= D[r][j] * k;
                        D[i][s] = D[r][s] * k;
                }
                for (int j = 0; j < n + 2; j++) if (j != s) D[r][j] *= iv;
                for (int i = 0; i < m + 2; i++) if (i != r) D[i][s] *= -iv;
                D[r][s] = iv;
                swap(B[r], N[s]);
        }

        bool simplex(int phase) {
                int x = m + phase - 1;
                while (true) {
                        int s = -1;
                        for (int j = 0; j <= n; j++) {
                                if (N[j] == -phase) continue;
                                if (s == -1 || make_pair(D[x][j], N[j]) < make_pair(D[x][s], N[s])) s = j;
                        }
                        if (D[x][s] >= -e) return true;
                        int r = -1;
                        for (int i = 0; i < m; i++) {
                                if (D[i][s] <= e) continue;
                                if (r == -1 || make_pair(D[i][n + 1] / D[i][s], B[i]) < make_pair(D[r][n + 1] / D[r][s], B[r])) r = i;
                        }
                        if (r == -1) return false;
                        pivot(r, s);
                }
        }

        ld solve(ve<ld> &x) {
                int r = 0;
                for (int i = 1; i < m; i++) if (D[i][n + 1] < D[r][n + 1]) r = i;
                if (D[r][n + 1] < -e) {
                        pivot(r, n);
                        if (!simplex(2) || D[m + 1][n + 1] < -e) return -inf;
                        for (int i = 0; i < m; i++) if (B[i] == -1) {
                                int s = 0;
                                for (int j = 1; j <= n; j++) {
                                        if (make_pair(D[i][j], N[j]) < make_pair(D[i][s], N[s])) s = j;
                                }
                                pivot(i, s);
                        }
                }
                bool ok = simplex(1);
                x.assign(n, 0);
                for (int i = 0; i < m; i++) if (B[i] < n) x[B[i]] = D[i][n + 1];
                return ok ? D[m][n + 1] : inf;
        }
};
