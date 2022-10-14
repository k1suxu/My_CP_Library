// no need to build
// range add query, range minimum(maximum) query
// funcionの内容を変えることで最大値も得られる。他のこともできる
struct RMQ_RAQ {
    //初期値
    const int e = 0;
    function<int(int, int)> fx = [](int x, int y) -> int {
        return min(x, y);
    };
    //const int INF = 2147483647;
    int n;
    vector<int> dat, lazy;
    RMQ_RAQ(int n_) : n(), dat(n_ * 4, e), lazy(n_ * 4, e) {
        int x = 1;
        while(n_ > x) {
            x *= 2;
        }
        n = x;
    }

    void evaluate(int k) {
        if(lazy[k] == 0) return;
        if(k < n - 1) {
            lazy[2 * k + 1] += lazy[k];
            lazy[2 * k + 2] += lazy[k];
        }
        dat[k] += lazy[k];
        lazy[k] = 0;
    }

    void add(int a, int b, int x) {
        add_sub(a, b, x, 0, 0, n);
    }
    void add_sub(int a, int b, int x, int k, int l, int r) {
        evaluate(k);
        if(r <= a || b <= l) {
            return;
        }else if(a <= l && r <= b) {
            lazy[k] += x;
            evaluate(k);
        }else {
            add_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            add_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
            dat[k] = fx(dat[k * 2 + 1], dat[k * 2 + 2]);
        }
    }

    int query(int a, int b) {
        return query_sub(a, b, 0, 0, n);
    }
    int query_sub(int a, int b, int k, int l, int r) {
        evaluate(k);
        if(r <= a || b <= l) {
            //限界値
            return INF;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            int vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            int vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
    }

    /* debug */
    inline int operator[](int i) {
        return query(i, i + 1);
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
struct RMQ_RAQ {
    //初期値
    const T e = TODO;
    const T zero = TODO;
    function<T(T, T)> fx = [](T x, T y) -> T {
        return TODO;
    };
    //const int INF = 2147483647;
    int n;
    vector<T> dat, lazy;
    RMQ_RAQ(int n_) : n(), dat(n_ * 4, e), lazy(n_ * 4, zero) {
        int x = 1;
        while(n_ > x) {
            x *= 2;
        }
        n = x;
    }
    RMQ_RAQ(int n_, vector<T> init) : n(), dat(n_*4, e), lazy(n_*4, zero) {
        int x = 1;
        while(n_ > x) x *= 2;
        n = x;

        for(int i = 0; i < init.size(); i++) {
            dat[i+n-1] = init[i];
        }
        for(int i = n-2; i >= 0; i--) {
            dat[i] = fx(dat[i*2+1], dat[i*2+2]);
        }
    }

    void evaluate(int k) {
        if(lazy[k] == zero) return;
        if(k < n - 1) {
            lazy[2 * k + 1] += lazy[k];
            lazy[2 * k + 2] += lazy[k];
        }
        dat[k] += lazy[k];
        lazy[k] = zero;
    }

    void add(int a, int b, T x) {
        add_sub(a, b, x, 0, 0, n);
    }
    void add_sub(int a, int b, T x, int k, int l, int r) {
        evaluate(k);
        if(r <= a || b <= l) {
            return;
        }else if(a <= l && r <= b) {
            lazy[k] += x;
            evaluate(k);
        }else {
            add_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            add_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
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
            return e;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            T vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
    }

    /* debug */
    inline T operator[](int i) {
        return query(i, i + 1);
    }
    void print() {
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n - 1) cout << ",";
        }
        cout << endl;
    }
};