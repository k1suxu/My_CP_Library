struct RMQ_RUQ {
    const int e = numeric_limits<int>::max();
    function<int(int, int)> fx = [](int x, int y) -> int {
        return min(x, y);
    };
    int n;
    vector<int> dat, lazy;
    RMQ_RUQ(int n_) : n(), dat(n_ * 4, e), lazy(n_ * 4, e) {
        int x = 1;
        while(n_ > x) x *= 2;
        n = x;
    }

    // evaluateするときは上から順にみてるため、
    // 更新するのは直系の子供だけでよい
    // それ以外の値についてはまた今度行えばいい。
    void evaluate(int k) {
        if(lazy[k] == e) return;
        if(k < n - 1) {
            lazy[k * 2 + 1] = lazy[k];
            lazy[k * 2 + 2] = lazy[k];
        }
        dat[k] = lazy[k];
        lazy[k] = e;
    }

    void update(int a, int b, int x) {
        update_sub(a, b, x, 0, 0, n);
    }

    void update_sub(int a, int b, int x, int k, int l, int r) {
        evaluate(k);
        if(a <= l && r <= b) {
            lazy[k] = x;
            evaluate(k);
        }else if(a < r && l < b) {
            update_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            update_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
            dat[k] = fx(dat[k * 2 + 1], dat[k * 2 + 2]);
        }
    }

    int query(int a, int b) {
        return query_sub(a, b, 0, 0, n);
    }

    int query_sub(int a, int b, int k, int l, int r) {
        evaluate(k);
        if(r <= a || b <= l) {
            return e;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            int vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            int vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
    }

    inline int operator[](int a) {return query(a, a + 1);}
    void print() {
        for(int i = 0; i < n; ++i) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << endl;
    }
};

//abstract
template<typename T>
struct RMQ_RUQ {
    const T e = TODO;
    //絶対に変更先に指定されないもの(不可能値)
    const T LIMIT = numeric_limits<int>::max();
    function<T(T, T)> fx = [](T x, T y) -> T {
        return TODO;
    };
    int n;
    vector<T> dat, lazy;
    RMQ_RUQ(int n_) : n(), dat(n_ * 4, e), lazy(n_ * 4, LIMIT) {
        int x = 1;
        while(n_ > x) x *= 2;
        n = x;
    }

    // evaluateするときは上から順にみてるため、
    // 更新するのは直系の子供だけでよい
    // それ以外の値についてはまた今度行えばいい。
    void evaluate(int k) {
        if(lazy[k] == LIMIT) return;
        if(k < n - 1) {
            lazy[k * 2 + 1] = lazy[k];
            lazy[k * 2 + 2] = lazy[k];
        }
        dat[k] = lazy[k];
        lazy[k] = LIMIT;
    }

    void update(int a, int b, T x) {
        update_sub(a, b, x, 0, 0, n);
    }

    void update_sub(int a, int b, T x, int k, int l, int r) {
        evaluate(k);
        if(a <= l && r <= b) {
            lazy[k] = x;
            evaluate(k);
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
        evaluate(k);
        if(r <= a || b <= l) {
            //限界値
            return TODO;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            T vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
    }

    inline T operator[](int a) {return query(a, a + 1);}
    void print() {
        for(int i = 0; i < n; ++i) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << endl;
    }
};