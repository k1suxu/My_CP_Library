// no need to build
// range add query, get 1 ingredient;
struct RAQ {
    int n;
    vector<int> dat;
    RAQ(int n_) : n(), dat(n_ * 4, 0) {
        int x = 1;
        while(n_ > x) {
            x *= 2;
        }
        n = x;
    }

    void add(int a, int b, int x) {add_sub(a, b, x, 0, 0, n);}
    void add_sub(int a, int b, int x, int k, int l, int r) {
        if(r <= a || b <= l) {
        }else if(a <= l && r <= b) {
            dat[k] += x;
        }else {
            add_sub(a, b, x, k * 2 + 1, l, (l + r) / 2);
            add_sub(a, b, x, k * 2 + 2, (l + r) / 2, r);
        }
    }

    int get(int i) {
        int res = 0;
        i += n - 1;
        res += dat[i];
        while(i) {
            i = (i - 1) / 2;
            res += dat[i];
        }
        return res;
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
