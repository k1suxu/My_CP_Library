// no need to build
// そのたびに値更新 -> like lazy RMQ
// range sum query
// add 1 sequence
// update 1 sequence
struct RSQ {
    int n;
    vector<int> dat;

    function<int(int, int)> fx = [&](int x, int y) {return (x + y);};

    RSQ(int n_) : n(), dat(n_ * 4, 0) {
        int x = 1;
        while(n_ > x) {
            x *= 2;
        }
        n = x;
    }

    void update(int i, int x) {
        i += n - 1;
        dat[i] = x;
        while(i) {
            i = (i - 1) / 2;
            dat[i] = fx(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }
    
    void add(int i, int x) {
        i += n - 1;
        dat[i] += x;
        while(i) {
            i = (i - 1) / 2;
            dat[i] = fx(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }

    // set(i, a) = add(i, a - get(i))
    int get(int i) {
        return dat[i + n - 1];
    }

    // [a, b) 右半開区間
    int query(int a, int b) {return query_sub(a, b, 0, 0, n);}
    int query_sub(int a, int b, int k, int l, int r) {
        if(r <= a || b <= l) {
            return 0;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            int vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            int vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
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