#pragma once
#include "tmp.cpp"

const int ALPHA = 26;  // алфавит 'a'..'z'

// Суффиксный автомат: len, link, переходы next. Различных подстрок = sum(len[v] - len[link[v]]).
struct SufAuto {
        struct State {
                int len = 0, link = -1;
                array<int, ALPHA> next;
                State() { next.fill(-1); }
        };

        ve<State> st;
        int last = 0;

        SufAuto() : st(1) {}
        SufAuto(const string &s) : st(1) {
                for (char c : s) add(c - 'a');
        }

        void add(int c) {
                int cur = st.size();
                st.emplace_back();
                st[cur].len = st[last].len + 1;
                int p = last;
                while (p != -1 && st[p].next[c] == -1) {
                        st[p].next[c] = cur;
                        p = st[p].link;
                }
                if (p == -1) st[cur].link = 0;
                else {
                        int q = st[p].next[c];
                        if (st[p].len + 1 == st[q].len) st[cur].link = q;
                        else {
                                int cl = st.size();
                                st.push_back(st[q]);
                                st[cl].len = st[p].len + 1;
                                while (p != -1 && st[p].next[c] == q) {
                                        st[p].next[c] = cl;
                                        p = st[p].link;
                                }
                                st[q].link = st[cur].link = cl;
                        }
                }
                last = cur;
        }

        ll distinct() {
                ll res = 0;
                for (size_t v = 1; v < st.size(); v++) res += st[v].len - st[st[v].link].len;
                return res;
        }
};

// Ахо-Корасик: add(слово) -> id; после build() go[v][c] -- полный автомат, cnt[v] -- число
// слов-суффиксов состояния (суммируется по суффиксным ссылкам).
struct AhoCorasick {
        ve<array<int, ALPHA>> go;
        vi link, cnt;

        AhoCorasick() {
                newNode();
        }

        int newNode() {
                go.emplace_back();
                go.back().fill(-1);
                link.push_back(0);
                cnt.push_back(0);
                return (int)go.size() - 1;
        }

        int add(const string &s) {
                int v = 0;
                for (char ch : s) {
                        int c = ch - 'a';
                        if (go[v][c] == -1) {
                                int u = newNode();
                                go[v][c] = u;
                        }
                        v = go[v][c];
                }
                cnt[v]++;
                return v;
        }

        void build() {
                queue<int> q;
                for (int c = 0; c < ALPHA; c++) {
                        if (go[0][c] == -1) go[0][c] = 0;
                        else q.push(go[0][c]);
                }
                while (!q.empty()) {
                        int v = q.front();
                        q.pop();
                        cnt[v] += cnt[link[v]];
                        for (int c = 0; c < ALPHA; c++) {
                                int u = go[v][c];
                                if (u == -1) go[v][c] = go[link[v]][c];
                                else {
                                        link[u] = go[link[v]][c];
                                        q.push(u);
                                }
                        }
                }
        }
};

// Palindromic tree (eertree): вершины 0 (len -1, корень нечётных), 1 (len 0), дальше -- палиндромы.
// Различных палиндромов-подстрок = size - 2. occ[v] после finish() -- число вхождений.
struct Eertree {
        struct Node {
                int len, link;
                array<int, ALPHA> next;
                ll occ = 0;
        };

        ve<Node> t;
        string s;
        int last = 1;

        Eertree() {
                t.push_back({-1, 0, {}, 0});
                t.push_back({0, 0, {}, 0});
                t[0].next.fill(-1);
                t[1].next.fill(-1);
        }

        int getLink(int v) {
                int i = s.size() - 1;
                while (i - t[v].len - 1 < 0 || s[i - t[v].len - 1] != s[i]) v = t[v].link;
                return v;
        }

        void add(char ch) {
                s += ch;
                int c = ch - 'a', cur = getLink(last);
                if (t[cur].next[c] == -1) {
                        int v = t.size();
                        t.push_back({t[cur].len + 2, 0, {}, 0});
                        t[v].next.fill(-1);
                        if (t[v].len == 1) t[v].link = 1;
                        else t[v].link = t[getLink(t[cur].link)].next[c];
                        t[cur].next[c] = v;
                }
                last = t[cur].next[c];
                t[last].occ++;
        }

        void finish() {  // накопить вхождения по суффиксным ссылкам
                for (int v = (int)t.size() - 1; v > 1; v--) t[t[v].link].occ += t[v].occ;
        }
};
