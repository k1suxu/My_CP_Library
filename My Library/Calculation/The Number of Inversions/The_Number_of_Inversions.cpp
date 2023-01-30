//転倒数を求める。
//int
struct RSQ {
    int n;
    vector<int> dat;

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
            dat[i] = dat[i * 2 + 1] + dat[i * 2 + 2];
        }
    }
    
    void add(int i, int x) {
        i += n - 1;
        dat[i] += x;
        while(i) {
            i = (i - 1) / 2;
            dat[i] = dat[i * 2 + 1] + dat[i * 2 + 2];
        }
    }

    // set(i, a) = add(i, a - get(i))
    int get(int i) {
        return dat[i + n - 1];
    }

    // [a, b) 左半開区間
    int query(int a, int b) {return query_sub(a, b, 0, 0, n);}
    int query_sub(int a, int b, int k, int l, int r) {
        if(r <= a || b <= l) {
            return 0;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            int vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            int vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return vl + vr;
        }
    }
};

template <typename T>
vector<T> compress(vector<T> &X) {
    vector<T> vals = X;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    for (int i = 0; i < (int)X.size(); i++) {
        X[i] = lower_bound(vals.begin(), vals.end(), X[i]) - vals.begin();
    }
    //vals[X[i]] = original X[i]
    return vals;
}

int the_number_of_inversions(vector<int> a) {
    int n = (int)a.size();
    compress(a);

    RSQ tree(n);
    int inversion = 0;

    FOR(n) {
        inversion += tree.query(a[i]+1, n+10);
        tree.add(a[i], 1);
    }

    return inversion;
}



