#pragma once
#include "math/numth.cpp"

// Расширенная матрица a (n x (m + 1)) по простому модулю p. Возвращает ранг матрицы
// коэффициентов, или -1 если система несовместна. x -- одно решение (свободные = 0).
int gaussMod(ve<vll> a, vll &x, ll p = MOD) {
        int n = a.size(), m = a[0].size() - 1, rk = 0;
        vi where(m, -1);
        for (int col = 0; col < m && rk < n; col++) {
                int sel = -1;
                for (int i = rk; i < n; i++) if (a[i][col] % p) { sel = i; break; }
                if (sel == -1) continue;
                swap(a[sel], a[rk]);
                where[col] = rk;
                ll iv = inv(a[rk][col], p);
                for (auto &v : a[rk]) v = v * iv % p;
                for (int i = 0; i < n; i++) {
                        if (i == rk || !a[i][col]) continue;
                        ll c = a[i][col];
                        for (int j = col; j <= m; j++) a[i][j] = ((a[i][j] - c * a[rk][j]) % p + p) % p;
                }
                rk++;
        }
        x.assign(m, 0);
        for (int i = 0; i < m; i++) if (where[i] != -1) x[i] = a[where[i]][m];
        for (int i = rk; i < n; i++) if (a[i][m] % p) return -1;
        return rk;
}

ll detMod(ve<vll> a, ll p = MOD) {
        int n = a.size();
        ll det = 1;
        for (int col = 0; col < n; col++) {
                int sel = -1;
                for (int i = col; i < n; i++) if (a[i][col] % p) { sel = i; break; }
                if (sel == -1) return 0;
                if (sel != col) swap(a[sel], a[col]), det = (p - det) % p;
                det = det * a[col][col] % p;
                ll iv = inv(a[col][col], p);
                for (int i = col + 1; i < n; i++) {
                        ll c = a[i][col] * iv % p;
                        for (int j = col; j < n; j++) a[i][j] = ((a[i][j] - c * a[col][j]) % p + p) % p;
                }
        }
        return det;
}

// Гаусс по вещественным: расширенная матрица, частичный выбор ведущего. Возвращает ранг / -1.
int gaussD(ve<ve<ld>> a, ve<ld> &x, ld eps = 1e-9) {
        int n = a.size(), m = a[0].size() - 1, rk = 0;
        vi where(m, -1);
        for (int col = 0; col < m && rk < n; col++) {
                int sel = rk;
                for (int i = rk; i < n; i++) if (abs(a[i][col]) > abs(a[sel][col])) sel = i;
                if (abs(a[sel][col]) < eps) continue;
                swap(a[sel], a[rk]);
                where[col] = rk;
                for (int i = 0; i < n; i++) {
                        if (i == rk) continue;
                        ld c = a[i][col] / a[rk][col];
                        for (int j = col; j <= m; j++) a[i][j] -= c * a[rk][j];
                }
                rk++;
        }
        x.assign(m, 0);
        for (int i = 0; i < m; i++) if (where[i] != -1) x[i] = a[where[i]][m] / a[where[i]][i];
        for (int i = rk; i < n; i++) if (abs(a[i][m]) > eps) return -1;
        return rk;
}

using Mat = ve<vll>;

Mat matMul(const Mat &a, const Mat &b, ll p = MOD) {
        int n = a.size(), m = b[0].size(), k = b.size();
        Mat c(n, vll(m));
        for (int i = 0; i < n; i++) for (int t = 0; t < k; t++) for (int j = 0; j < m; j++) {
                c[i][j] = (c[i][j] + a[i][t] * b[t][j]) % p;
        }
        return c;
}

Mat matPow(Mat a, ll e, ll p = MOD) {
        int n = a.size();
        Mat r(n, vll(n));
        for (int i = 0; i < n; i++) r[i][i] = 1;
        for (; e; e >>= 1, a = matMul(a, a, p)) if (e & 1) r = matMul(r, a, p);
        return r;
}
