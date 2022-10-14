template<typename T>
struct RSQ_RAQ {
    const T e = TODO;
    const T zero = TODO;
    vector<T> dat, lazy;
    int n;
    function<T(T, T)> fx = [](T x, T y) {return x + y;};

    RSQ_RAQ(int n_) : n(), dat(4*n_, e), lazy(4*n_, zero) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void evaluate(int length, int k) {
        if(lazy[k] == zero) return;
        if(k <= n - 2) {
            lazy[k * 2 + 1] += lazy[k];
            lazy[k * 2 + 2] += lazy[k];
        }
        dat[k] += lazy[k] * length;
        lazy[k] = zero;
    }

    void add(int a, int b, T x) {
        return add_sub(a, b, x, 0, 0, n);
    }
    void add_sub(int a, int b, T x, int k, int l, int r) {
        evaluate(r - l, k);
        if(a <= l && r <= b) {
            lazy[k] += x;
            evaluate(r - l, k);
        }else if(a < r && l < b) {
            add_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            add_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
            dat[k] = fx(dat[k * 2 + 1], dat[k * 2 + 2]);
        }
    }

    T query(int a, int b) {
        return query_sub(a, b, 0, 0, n);
    }
    T query_sub(int a, int b, int k, int l, int r) {
        evaluate(r - l, k);
        if(r <= a || b <= l) {
            return e;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            T vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
    }

    inline T operator[](int i) {
        return query(i, i + 1);
    }
    void print() {
        for(int i = 0; i < n; ++i) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << endl;
    }
};