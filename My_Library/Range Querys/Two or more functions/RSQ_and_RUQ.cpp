template<typename T>
struct RSQ_RUQ {
    const T e = 0;
    //絶対に変更先に指定されないもの(不可能値)
    const T LIMIT = numeric_limits<int>::max();
    vector<T> dat, lazy;
    int n;
    function<T(T, T)> fx = [](T x, T y) {return x + y;};

    RSQ_RUQ(int n_) : n(), dat(4*n_, e), lazy(4*n_, LIMIT) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void evaluate(int length, int k) {
        if(lazy[k] == LIMIT) return;
        if(k <= n - 2) {
            lazy[k * 2 + 1] = lazy[k];
            lazy[k * 2 + 2] = lazy[k];
        }
        dat[k] = lazy[k] * length;
        lazy[k] = LIMIT;
    }

    void update(int a, int b, T x) {
        return update_sub(a, b, x, 0, 0, n);
    }
    void update_sub(int a, int b, T x, int k, int l, int r) {
        evaluate(r - l, k);
        if(a <= l && r <= b) {
            lazy[k] = x;
            evaluate(r - l, k);
        }else if(a < r && l < b) {
            update_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            update_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
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
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        } cout << "\n";
    }
};