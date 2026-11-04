#pragma once
#include "tmp.cpp"

// Алгоритм Mo: оффлайн запросы [l, r) к массиву. add(i)/del(i) -- добавить/убрать элемент,
// get() -- ответ на текущем отрезке. O((n + q) sqrt n).
template <typename Add, typename Del, typename Get>
vll mo(int n, const ve<pii> &qs, Add add, Del del, Get get) {
        int q = qs.size(), B = max(1, (int)(n / sqrt((double)max(q, 1))));
        vi ord(q);
        iota(all(ord), 0);
        sort(all(ord), [&](int a, int b) {
                int ba = qs[a].first / B, bb = qs[b].first / B;
                if (ba != bb) return ba < bb;
                return (ba & 1) ? qs[a].second > qs[b].second : qs[a].second < qs[b].second;
        });
        vll res(q);
        int l = 0, r = 0;
        for (int id : ord) {
                while (l > qs[id].first) add(--l);
                while (r < qs[id].second) add(r++);
                while (l < qs[id].first) del(l++);
                while (r > qs[id].second) del(--r);
                res[id] = get();
        }
        return res;
}
