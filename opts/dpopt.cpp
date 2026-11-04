#pragma once
#include "tmp.cpp"

// Оптимизация D&C: dp[j] = min_{k < j} prev[k] + cost(k, j), opt(j) монотонен
// (достаточно неравенства четырёхугольника на cost). O(n log n) на слой.
// prev[k] = INFLL -- недостижимо. Слои: for (i) cur = dcLayer(prev, cost).
template <typename F>
void dcRec(const vll &prev, vll &cur, int l, int r, int optl, int optr, F &cost) {
        if (l > r) return;
        int mid = (l + r) / 2, opt = -1;
        ll best = INFLL;
        for (int k = optl; k <= min(mid - 1, optr); k++) {
                if (prev[k] >= INFLL) continue;
                ll v = prev[k] + cost(k, mid);
                if (v < best) best = v, opt = k;
        }
        cur[mid] = best;
        if (opt == -1) opt = optl;
        dcRec(prev, cur, l, mid - 1, optl, opt, cost);
        dcRec(prev, cur, mid + 1, r, opt, optr, cost);
}

template <typename F>
vll dcLayer(const vll &prev, F cost) {
        int n = prev.size();
        vll cur(n, INFLL);
        dcRec(prev, cur, 0, n - 1, 0, n - 1, cost);
        return cur;
}

// Кнут: dp[l][r] = min_{l <= k < r} dp[l][k] + dp[k + 1][r] + cost(l, r), dp[i][i] = 0.
// Нужны QI и монотонность cost по вложению. O(n^2). Возвращает dp[0][n - 1].
template <typename F>
ll knuth(int n, F cost) {
        ve<vll> dp(n, vll(n, 0));
        ve<vi> opt(n, vi(n, 0));
        for (int i = 0; i < n; i++) opt[i][i] = i;
        for (int len = 2; len <= n; len++) {
                for (int l = 0; l + len <= n; l++) {
                        int r = l + len - 1;
                        dp[l][r] = INFLL;
                        for (int k = opt[l][r - 1]; k <= min(r - 1, opt[l + 1][r]); k++) {
                                ll v = dp[l][k] + dp[k + 1][r] + cost(l, r);
                                if (v < dp[l][r]) dp[l][r] = v, opt[l][r] = k;
                        }
                }
        }
        return dp[0][n - 1];
}

// НВП (строго возрастающая): длина и сама подпоследовательность (индексы), O(n log n)
vi lis(const vi &a) {
        int n = a.size();
        vi d, idx, par(n, -1);
        for (int i = 0; i < n; i++) {
                int p = lower_bound(all(d), a[i], [&](int j, int x) { return a[j] < x; }) - d.begin();
                if (p == (int)d.size()) d.push_back(i);
                else d[p] = i;
                par[i] = p ? d[p - 1] : -1;
        }
        vi res;
        for (int v = d.empty() ? -1 : d.back(); v != -1; v = par[v]) res.push_back(v);
        reverse(all(res));
        return res;
}
