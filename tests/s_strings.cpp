#include "common.h"
#include "strings/strings.cpp"
#include "strings/automata.cpp"

string rs(int n, int k) { string s(n, 'a'); for (auto &c : s) c = 'a' + rnd(0, k - 1); return s; }

signed main() {
        for (int it = 0; it < 6000; it++) {
                int n = rnd(1, 20), k = rnd(1, 3);
                string s = rs(n, k);
                // z, pref
                auto z = z_func(s); auto p = pref_func(s);
                for (int i = 0; i < n; i++) {
                        int l = 0; while (i + l < n && s[l] == s[i + l]) l++;
                        CHECK_EQ(z[i], i ? l : 0);
                        int best = 0; for (int len = 1; len <= i; len++) if (s.substr(0, len) == s.substr(i + 1 - len, len)) best = len;
                        CHECK_EQ(p[i], best);
                }
                // манакер
                auto [d1, d2] = manacher(s);
                for (int i = 0; i < n; i++) {
                        int c1 = 0, c2 = 0;
                        for (int l = 0; l < n; l++) for (int r = l; r < n; r++) {
                                string t = s.substr(l, r - l + 1), u = t; reverse(all(u));
                                if (t != u) continue;
                                if ((l + r) % 2 == 0 && (l + r) / 2 == i) c1++;
                                if ((l + r) % 2 == 1 && (l + r + 1) / 2 == i) c2++;
                        }
                        CHECK_EQ(d1[i], c1); CHECK_EQ(d2[i], c2);
                }
                // мин. циклический сдвиг
                int mr = 0; string best = s;
                for (int i = 0; i < n; i++) { string t = s.substr(i) + s.substr(0, i); if (t < best) best = t; }
                CHECK_EQ(s.substr(minRotation(s)) + s.substr(0, minRotation(s)), best);
                // хеш
                Hash61 h(s);
                for (int a = 0; a < n; a++) for (int b = a + 1; b <= n; b++) for (int c = 0; c < n; c++) for (int d = c + 1; d <= n; d++) {
                        if (b - a != d - c) continue;
                        CHECK_EQ(h.get(a, b) == h.get(c, d), s.substr(a, b - a) == s.substr(c, d - c));
                }
                // суфмас + lcp
                vi sa = suffixArray(s), lcp = lcpArray(s, sa);
                vi ex(n); iota(all(ex), 0);
                sort(all(ex), [&](int a, int b) { return s.substr(a) < s.substr(b); });
                CHECK_EQ(sa, ex);
                for (int i = 0; i + 1 < n; i++) { int l = 0; while (sa[i] + l < n && sa[i + 1] + l < n && s[sa[i] + l] == s[sa[i + 1] + l]) l++; CHECK_EQ(lcp[i], l); }
                // автомат
                SufAuto sam(s);
                set<string> subs; for (int l = 0; l < n; l++) for (int r = l + 1; r <= n; r++) subs.insert(s.substr(l, r - l));
                CHECK_EQ(sam.distinct(), (ll)subs.size());
                for (int q = 0; q < 10; q++) {
                        string t = rs(rnd(1, 6), k); int v = 0; bool ok = true;
                        for (char ch : t) { v = sam.st[v].next[ch - 'a']; if (v == -1) { ok = false; break; } }
                        CHECK_EQ(ok, (bool)subs.count(t));
                }
                // эертри
                Eertree et; for (char c : s) et.add(c); et.finish();
                set<string> pals; map<string, ll> occ;
                for (int l = 0; l < n; l++) for (int r = l + 1; r <= n; r++) { string t = s.substr(l, r - l), u = t; reverse(all(u)); if (t == u) pals.insert(t), occ[t]++; }
                CHECK_EQ((int)et.t.size() - 2, (int)pals.size());
                ll tot = 0; for (size_t v = 2; v < et.t.size(); v++) tot += et.t[v].occ;
                ll exp = 0; for (auto &[t, c] : occ) exp += c;
                CHECK_EQ(tot, exp);
                // Ахо-Корасик
                AhoCorasick ac; int w = rnd(1, 4); ve<string> words;
                for (int i = 0; i < w; i++) { words.push_back(rs(rnd(1, 4), k)); ac.add(words.back()); }
                ac.build();
                ll got = 0, expc = 0; int v = 0;
                for (char c : s) { v = ac.go[v][c - 'a']; got += ac.cnt[v]; }
                for (auto &wd : words) for (int i = 0; i + (int)wd.size() <= n; i++) if (s.compare(i, wd.size(), wd) == 0) expc++;
                CHECK_EQ(got, expc);
        }
        cout << "ok\n";
}
