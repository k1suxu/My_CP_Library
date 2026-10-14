// https://atcoder.jp/contests/abc454/tasks/abc454_f

int cal(vector<int> a, int m) {
    const int n = (int)a.size();
    for (auto &e : a) e = (e % m + m) % m;

    vector<int> t;
    t.reserve(n + 1);

    int prv = 0;
    a.push_back(0);
    for (int i = 0; i <= n; i++) {
        int diff = (a[i] - prv + m) % m;
        prv = a[i];

        int x = diff;
        if (x > m / 2) x -= m;
        t.push_back(x);
    }

    int l0 = 0, s = 0;
    for (auto x : t) {
        l0 += abs(x);
        s += x;
    }

    int q = s / m;

    vi cost;
    if (q > 0) {
        for (auto x : t) {
            int delta = (x >= 0? m - 2 * x : m);
            cost.push_back(delta);
        }
        nth_element(cost.begin(), cost.begin() + q, cost.end());
        for (int i = 0; i < q; i++) l0 += cost[i];
    } else if (q < 0) {
        q = -q;

        for (auto x : t) {
            int delta = (x > 0? m : m + 2 * x);
            cost.push_back(delta);
        }
        nth_element(cost.begin(), cost.begin() + q, cost.end());
        for (int i = 0; i < q; i++) l0 += cost[i];
    }

    return l0 / 2;
}

// ソートありver.
int cal(vector<int> a, int m) {
    a.insert(a.begin(), 0);
    a.push_back(0);

    const int n = (int)a.size();
    vector<int> b(n-1);
    for (int i = 0; i < n-1; i++) {
        b[i] = ((a[i+1] - a[i]) % m + m) % m;
    }
    sort(all(b));

    int use = n - 1 - (accumulate(all(b), 0LL) / m);
    return accumulate(b.begin(), b.begin() + use, 0LL);
}