struct chmin_tree {
    vector<int> dat;
    int n;

    chmin_tree(int n_) : n(), dat(4*n_, INF) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void change_min(int a, int b, int x) {
        return change_min_sub(a, b, x, 0, 0, n);
    }
    void change_min_sub(int a, int b, int x, int k, int l, int r) {
        if(r <= a || b <= l) {
            return;
        }else if(a <= l && r <= b) {
            chmin(dat[k], x);
        }else {
            change_min_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            change_min_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
        }
    }

    int get(int i) {
        i += n - 1;
        int ret = dat[i];
        while(i > 0) {
            i = (i - 1) / 2;
            chmin(ret, dat[i]);
        }
        return ret;
    }

    inline int operator[](int i) {
        return get(i);
    }
    void print() {
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << endl;
    }
};

template<typename T>
struct chmin_tree {
    vector<T> dat;
    int n;

    chmin_tree(int n_) : n(), dat(4*n_, 0) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void change_min(int a, int b, T x) {
        return change_min_sub(a, b, x, 0, 0, n);
    }
    void change_min_sub(int a, int b, T x, int k, int l, int r) {
        if(r <= a || b <= l) {
            return;
        }else if(a <= l && r <= b) {
            chmin(dat[k], x);
        }else {
            change_min_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            change_min_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
        }
    }

    int get(int i) {
        i += n - 1;
        int ret = dat[i];
        while(i > 0) {
            i = (i - 1) / 2;
            chmin(ret, dat[i]);
        }
        return ret;
    }

    inline int operator[](int i) {
        return get(i);
    }
    void print() {
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << endl;
    }
};