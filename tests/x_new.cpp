#include "common.h"
#include "opts/poly.cpp"
#include "dataStructures/mo.cpp"
#include "graphs/dyncon.cpp"
#include "geoma/polygon.cpp"
#include "other/misc.cpp"

signed main() {
        // многочлены
        for (int it = 0; it < 300; it++) {
                int n = rnd(1, 40);
                poly a(n); for (auto &x : a) x = rnd(0, PM - 1);
                if (!a[0]) a[0] = 1;
                poly b = polyInv(a, n), p = conv(a, b);
                p.resize(n);
                for (int i = 0; i < n; i++) CHECK_EQ(p[i], i == 0 ? 1 : 0);
                // exp / log: exp(a) через ОДУ b' = a' b : n b_n = sum k a_k b_{n-k}
                poly c = a; c[0] = 0;
                poly e = polyExp(c, n), ex(n);
                ex[0] = 1;
                for (int i = 1; i < n; i++) { ll s = 0; for (int k = 1; k <= i; k++) s = (s + (ll)k * c[k] % PM * ex[i - k]) % PM; ex[i] = s * inv(i, PM) % PM; }
                CHECK_EQ(e, ex);
                poly l = polyLog(e, n);
                CHECK_EQ(l, c);
        }
        // числа Стирлинга
        { int N = 60; ve<vll> s1(N + 1, vll(N + 1)), s2(N + 1, vll(N + 1));
          s1[0][0] = s2[0][0] = 1;
          for (int n = 1; n <= N; n++) for (int k = 1; k <= n; k++) { s1[n][k] = (s1[n - 1][k - 1] + (n - 1) * s1[n - 1][k]) % PM; s2[n][k] = (s2[n - 1][k - 1] + (ll)k * s2[n - 1][k]) % PM; }
          for (int n = 0; n <= N; n++) { auto r1 = stirling1Row(n), r2 = stirling2Row(n);
                  for (int k = 0; k <= n; k++) { CHECK_EQ((ll)r1[k], s1[n][k]); CHECK_EQ((ll)r2[k], s2[n][k]); } } }
        // Mo: число различных на отрезке
        for (int it = 0; it < 500; it++) {
                int n = rnd(1, 30), q = rnd(1, 30);
                vi a(n); for (auto &x : a) x = rnd(0, 6);
                ve<pii> qs; for (int i = 0; i < q; i++) { int l = rnd(0, n - 1), r = rnd(l + 1, n); qs.push_back({l, r}); }
                vi cnt(7); int d = 0;
                auto res = mo(n, qs, [&](int i) { if (cnt[a[i]]++ == 0) d++; }, [&](int i) { if (--cnt[a[i]] == 0) d--; }, [&]() { return (ll)d; });
                for (int i = 0; i < q; i++) { set<int> s(a.begin() + qs[i].first, a.begin() + qs[i].second); CHECK_EQ(res[i], (ll)s.size()); }
        }
        // оффлайн динамическая связность
        for (int it = 0; it < 1000; it++) {
                int n = rnd(1, 8), T = rnd(1, 10), m = rnd(0, 12);
                OfflineConn oc(n, T); ve<array<int, 4>> es;
                for (int i = 0; i < m; i++) { int l = rnd(0, T - 1), r = rnd(l + 1, T), u = rnd(0, n - 1), v = rnd(0, n - 1); es.push_back({l, r, u, v}); oc.addEdge(l, r, u, v); }
                auto ans = oc.run();
                for (int t = 0; t < T; t++) { vi p(n); iota(all(p), 0); function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
                        for (auto &e : es) if (e[0] <= t && t < e[1]) p[f(e[2])] = f(e[3]);
                        int c = 0; for (int v = 0; v < n; v++) c += f(v) == v; CHECK_EQ(ans[t], c); }
        }
        // площадь круг ∩ выпуклый многоугольник: сравнение с отсечением многоугольником-приближением окружности
        for (int it = 0; it < 300; it++) {
                int n = rnd(3, 7); ve<pt> pts;
                for (int i = 0; i < n; i++) pts.push_back(pt(rnd(-8, 8), rnd(-8, 8)));
                auto h = hull(pts); if (h.size() < 3) continue;
                pt c(rnd(-4, 4), rnd(-4, 4)); ld r = rnd(1, 6);
                ld got = circlePoly(h, c, r);
                int M = 4000; ve<HP> H;
                for (size_t i = 0; i < h.size(); i++) H.push_back(HP(h[i], h[(i + 1) % h.size()] - h[i]));
                for (int i = 0; i < M; i++) { ld a1 = 2 * PI * i / M, a2 = 2 * PI * (i + 1) / M; pt p1 = c + pt(cosl(a1), sinl(a1)) * r, p2 = c + pt(cosl(a2), sinl(a2)) * r; H.push_back(HP(p1, p2 - p1)); }
                auto res = hpi(H); ld exp = res.empty() ? 0 : area2(res) / 2;
                CHECK(fabsl(got - exp) < 2e-3 * (PI * r * r) + 1e-6);
        }
        // misc: ordered set, быстрый ввод не тестируем (stdin), __int128
        { oset<pii> s; s.insert({5, 0}); s.insert({3, 1}); s.insert({5, 2}); CHECK_EQ((int)s.order_of_key({5, 0}), 1); CHECK_EQ(s.find_by_order(2)->second, 2);
          CHECK_EQ(toString((__int128)1 << 100), string("1267650600228229401496703205376")); CHECK_EQ(toString(-(__int128)5), string("-5"));
          unordered_map<ll, int, SplitMix> mp; for (int i = 0; i < 1000; i++) mp[i * 1000003LL] = i; CHECK_EQ(mp[5 * 1000003LL], 5); }
        cout << "ok\n";
}
