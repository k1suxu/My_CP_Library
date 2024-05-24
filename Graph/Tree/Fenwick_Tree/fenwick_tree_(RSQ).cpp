//int
struct fenwick_tree {
    int n, sz;
    vector<int> dat;
    function<int(int, int)> f = [](int x, int y) {return x + y;};

    fenwick_tree(int n_) : dat(4*n_, 0), sz(n_) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void add(int i, int x) {
        i += n - 1;
        dat[i] += x;
        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = f(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }

    void insert(int i) {
        return add(i, 1);
    }

    void erase(int i) {
        return add(i, -1);
    }

    void update(int i, int x) {
        i += n - 1;
        dat[i] = x;
        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = f(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }

    int get_sum(int a, int b) {
        return get_sum_sub(a, b, 0, 0, n);
    }
    int get_sum_sub(int a, int b, int k, int l, int r) {
        if(r <= a || b <= l) {
            return 0;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            int vl = get_sum_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            int vr = get_sum_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return f(vl, vr);
        }
    }

    int get(int i) {
        return dat[i + n - 1];
    }

    inline void print() {
        cout << "{ ";
        for(int i = 0; i < sz; i++) {
            cout << dat[i + n - 1] << " ";
        }
        cout << "}\n";
    }
};

//abstract
template<class Abel>
struct fenwick_tree {
    int n, sz;
    vector<Abel> dat;
    function<Abel(Abel, Abel)> f = [](Abel x, Abel y) {return x + y;};
    Abel INITIAL;

    fenwick_tree(int n_, Abel init) : dat(4*n_, init), INITIAL(init), sz(n_) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void add(int i, Abel x) {
        i += n - 1;
        dat[i] += x;
        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = f(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }

    void update(int i, Abel x) {
        i += n - 1;
        dat[i] = x;
        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = f(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }

    // void insert(int i) {
    //     return add(i, 1);
    // }

    // void erase(int i) {
    //     return add(i, -1);
    // }

    Abel get_sum(int a, int b) {
        return get_sum_sub(a, b, 0, 0, n);
    }
    Abel get_sum_sub(int a, int b, int k, int l, int r) {
        if(r <= a || b <= l) {
            return INITIAL;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            Abel vl = get_sum_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            Abel vr = get_sum_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return f(vl, vr);
        }
    }

    Abel get(int i) {
        return dat[i + n - 1];
    }

    inline void print() {
        cout << "{ ";
        for(int i = 0; i < sz; i++) {
            cout << dat[i + n - 1] << " ";
        }
        cout << "}\n";
    }
};