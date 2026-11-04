#include "common.h"
#include "geoma/polygon.cpp"

ipt rp(int R) { return ipt(rnd(-R, R), rnd(-R, R)); }
bool inTri(ipt p, ipt a, ipt b, ipt c) {  // замкнутый невырожденный треугольник
        ll s = (b - a) % (c - a); if (s == 0) return false;
        ll d1 = (b - a) % (p - a), d2 = (c - b) % (p - b), d3 = (a - c) % (p - c);
        return s > 0 ? (d1 >= 0 && d2 >= 0 && d3 >= 0) : (d1 <= 0 && d2 <= 0 && d3 <= 0);
}

signed main() {
        // выпуклая оболочка
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 9), R = rnd(1, 5);
                ve<ipt> a; for (int i = 0; i < n; i++) a.push_back(rp(R));
                auto h = hull(a);
                ve<ipt> u = a; sort(all(u)); u.erase(unique(all(u)), u.end());
                set<pll> ex;
                for (auto p : u) {
                        bool ext = true; int m = u.size();
                        for (int i = 0; i < m && ext; i++) for (int j = 0; j < m && ext; j++) {
                                if (u[i] == p || u[j] == p) continue;
                                if (onSeg(p, u[i], u[j])) ext = false;
                                for (int k = 0; k < m && ext; k++) if (!(u[k] == p) && inTri(p, u[i], u[j], u[k])) ext = false;
                        }
                        if (ext) ex.insert({p.x, p.y});
                }
                set<pll> got; for (auto p : h) got.insert({p.x, p.y});
                CHECK_EQ(got, ex);
                if (h.size() >= 3) {
                        CHECK(area2(h) > 0);
                        for (size_t i = 0; i < h.size(); i++) for (auto q : u) CHECK((h[(i + 1) % h.size()] - h[i]) % (q - h[i]) >= 0);
                        // Пик: 2S = 2I + B - 2 (площадь + pip)
                        ll I = 0, B = 0;
                        for (ll x = -R; x <= R; x++) for (ll y = -R; y <= R; y++) { int r = pip(h, ipt(x, y)); I += r == 2; B += r == 1; }
                        CHECK_EQ(area2(h), 2 * I + B - 2);
                        for (ll x = -R - 1; x <= R + 1; x++) for (ll y = -R - 1; y <= R + 1; y++) CHECK_EQ(pinConvex(h, ipt(x, y)), pip(h, ipt(x, y)) >= 1);
                }
                // диаметр и ближайшая пара
                ll best = 0; for (auto p : u) for (auto q : u) best = max(best, (p - q).len2());
                CHECK_EQ(diameter2(h), best);
                if (a.size() >= 2) { ll md = LLONG_MAX; for (size_t i = 0; i < a.size(); i++) for (size_t j = i + 1; j < a.size(); j++) md = min(md, (a[i] - a[j]).len2()); CHECK_EQ(closestPair(a), md); }
                // Минковский
                int m = rnd(1, 7); ve<ipt> b; for (int i = 0; i < m; i++) b.push_back(rp(R));
                auto hb = hull(b); ve<ipt> sums; for (auto p : h) for (auto q : hb) sums.push_back(p + q);
                auto ms = minkowski(h, hb), hs = hull(sums);
                CHECK_EQ(hull(ms), hs);
        }
        // звёздчатый многоугольник: pip против разбиения на веер
        for (int it = 0; it < 3000; it++) {
                int n = rnd(3, 8), R = 6; ve<ipt> v;
                set<ll> usedAng; ve<pair<ld, ipt>> tmp;
                while ((int)tmp.size() < n) { ipt p = rp(R); if (p.x == 0 && p.y == 0) continue; ld ang = atan2l(p.y, p.x); bool dup = false; for (auto &t : tmp) if (fabsl(t.first - ang) < 1e-9) dup = true; if (!dup) tmp.push_back({ang, p}); }
                sort(all(tmp)); for (auto &t : tmp) v.push_back(t.second);
                bool star = true; for (int i = 0; i < n; i++) if (v[i] % v[(i + 1) % n] <= 0) star = false;
                if (!star) continue;
                for (int q = 0; q < 40; q++) {
                        ipt p = rp(R + 1);
                        int exp = 0; bool onB = false, inside = false;
                        for (int i = 0; i < n; i++) { if (onSeg(p, v[i], v[(i + 1) % n])) onB = true; if (inTri(p, ipt(0, 0), v[i], v[(i + 1) % n])) inside = true; }
                        exp = onB ? 1 : (inside ? 2 : 0);
                        CHECK_EQ(pip(v, p), exp);
                }
        }
        // пересечение отрезков
        for (int it = 0; it < 30000; it++) {
                ipt a = rp(3), b = rp(3), c = rp(3), d = rp(3);
                // независимая реализация: параметрическая
                ll rr = (b - a) % (d - c); bool ex;
                if (rr != 0) {
                        ll t = (c - a) % (d - c), u = (c - a) % (b - a);
                        auto in01 = [&](ll num) { return rr > 0 ? (num >= 0 && num <= rr) : (num <= 0 && num >= rr); };
                        ex = in01(t) && in01(u);
                } else {
                        ex = false;
                        if ((c - a) % (b - a) == 0 && (d - a) % (b - a) == 0 + 0 || ((b - a).len2() == 0 && 0)) {}
                        // перебор точек решётки на обоих отрезках (вырожденные и коллинеарные случаи)
                        for (ll x = -3; x <= 3 && !ex; x++) for (ll y = -3; y <= 3; y++) if (onSeg(ipt(x, y), a, b) && onSeg(ipt(x, y), c, d)) { ex = true; break; }
                }
                if (rr != 0 || true) CHECK_EQ(segInter(a, b, c, d), ex);
        }
        // пересечения прямая/окружность/окружность, касательные
        for (int it = 0; it < 20000; it++) {
                ipt c1 = rp(4), c2 = rp(4); ll r1 = rnd(0, 5), r2 = rnd(0, 5);
                cr A(pt(c1.x, c1.y), r1), B(pt(c2.x, c2.y), r2);
                if (c1.x == c2.x && c1.y == c2.y) continue;
                ll d2 = (c1 - c2).len2();
                int expN = (d2 > (r1 + r2) * (r1 + r2) || d2 < (r1 - r2) * (r1 - r2)) ? 0 : (d2 == (r1 + r2) * (r1 + r2) || d2 == (r1 - r2) * (r1 - r2)) ? 1 : 2;
                if (r1 == 0 || r2 == 0) continue;
                auto pts = A.fcr(B);
                CHECK_EQ((int)pts.size(), expN);
                for (auto p : pts) { CHECK(fabsl(pt(A.c, p).len() - r1) < 1e-6); CHECK(fabsl(pt(B.c, p).len() - r2) < 1e-6); }
                ipt p0 = rp(6), p1 = rp(6); if (p0.x == p1.x && p0.y == p1.y) continue;
                ln l(pt(p0.x, p0.y), pt(p1.x, p1.y));
                ll cr_ = (p1 - p0) % (c1 - p0); ll dist2num = cr_ * cr_, dd = (p1 - p0).len2();
                int e2 = dist2num > r1 * r1 * dd ? 0 : dist2num == r1 * r1 * dd ? 1 : 2;
                auto lp = A.fln(l);
                CHECK_EQ((int)lp.size(), e2);
                for (auto p : lp) { CHECK(fabsl(pt(A.c, p).len() - r1) < 1e-6); CHECK(fabsl(l.g(p)) < 1e-6 * l.nr().len()); }
                auto tp = A.tang(pt(p0.x, p0.y));
                ll dp = (p0 - c1).len2(); int e3 = dp < r1 * r1 ? 0 : dp == r1 * r1 ? 1 : 2;
                CHECK_EQ((int)tp.size(), e3);
                for (auto p : tp) { CHECK(fabsl(pt(A.c, p).len() - r1) < 1e-6); if (e3 == 2) CHECK(fabsl(pt(A.c, p) * pt(p, pt(p0.x, p0.y))) < 1e-6); }
        }
        // описанная окружность, мин. покрывающая окружность
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 8); ve<pt> p; ve<ipt> ip;
                for (int i = 0; i < n; i++) { ipt q = rp(5); ip.push_back(q); p.push_back(pt(q.x, q.y)); }
                cr mc = minCircle(p);
                for (auto &q : p) CHECK(pt(mc.c, q).len() <= mc.r + 1e-7);
                ld best = 1e18;
                auto test = [&](pt c, ld r) { for (auto &q : p) if (pt(c, q).len() > r + 1e-7) return; best = min(best, r); };
                for (int i = 0; i < n; i++) { test(p[i], 0); for (int j = i + 1; j < n; j++) { test((p[i] + p[j]) / 2, pt(p[i], p[j]).len() / 2);
                        for (int k = j + 1; k < n; k++) { if (sgn((p[j] - p[i]) % (p[k] - p[i])) == 0) continue; cr c = circum(p[i], p[j], p[k]); test(c.c, c.r); } } }
                CHECK(fabsl(mc.r - best) < 1e-6);
        }
        // пересечение полуплоскостей против перебора вершин
        for (int it = 0; it < 4000; it++) {
                int n = rnd(1, 6); ve<HP> H; ve<ln> L;
                auto add = [&](pt p, pt d) { H.push_back(HP(p, d)); L.push_back(ln(p, p + d)); };
                for (int i = 0; i < n; i++) { ipt p = rp(4), d = rp(4); if (d.x == 0 && d.y == 0) { i--; continue; } add(pt(p.x, p.y), pt(d.x, d.y)); }
                int B = 20; add(pt(-B, -B), pt(1, 0)); add(pt(B, -B), pt(0, 1)); add(pt(B, B), pt(-1, 0)); add(pt(-B, B), pt(0, -1));
                auto res = hpi(H);
                ve<pt> cand;
                for (size_t i = 0; i < H.size(); i++) for (size_t j = i + 1; j < H.size(); j++) {
                        if (parallel(L[i], L[j])) continue;
                        pt q = inter(L[i], L[j]); bool ok = true;
                        for (auto &h : H) if (sgn(h.d % (q - h.p)) < -1e-7 * 0 && h.d % (q - h.p) < -1e-7) ok = false;
                        if (ok) cand.push_back(q);
                }
                ld ar = cand.empty() ? 0 : area2(hull(cand)) / 2, ar2 = res.empty() ? 0 : area2(res) / 2;
                CHECK(fabsl(ar - ar2) < 1e-5);
        }
        cout << "ok\n";
}
