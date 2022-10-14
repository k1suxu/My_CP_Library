// no need to build
// range update query, get 1 ingredient;
struct RUQ {
    int n;
    vector<pair<int, int>> dat;
    // dat.first = 更新したときのt
    // dat.second = 更新の値
    int t;

    RUQ(int n_) {
        dat = vector<pair<int, int>>(n_ * 4, make_pair(-1, 0));
        t = 0;
        int x = 1;
        while(n_ > x) {
            x *= 2;
        }
        n = x;
    }

    void set(int i, int x) {dat[i + n - 1].second = x;}

    void update(int a, int b, int x) {
        update_sub(a, b, x, 0, 0, n);
        t++;
    }
    void update_sub(int a, int b, int x, int k, int l, int r) {
        if(r <= a || b <= l) {
            return;
        }else if(a <= l && r <= b) {
            dat[k] = make_pair(t, x);
        }else {
            update_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            update_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
        }
    }

    int get(int i) {
        pair<int, int> res;
        i += n - 1;
        res = dat[i];
        while(i) {
            i = (i - 1) / 2;
            res = max(res, dat[i]);
        }
        return res.second;
    }

    inline int operator[](int i) {
        return get(i);
    }

    void print() {
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << "\n";
    }
};
