template<typename T>
struct Abstruct_SegmentTree {
    function<T(T, T)> f;
    const T E;

    vector<T> dat;
    int n;

    Abstruct_SegmentTree(int n_, T e, function<T(T, T)> F) : E(e), dat(4*n_, e) {
        f = F;
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void update(int i, T x) {
        i += n - 1;
        dat[i] = x;

        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = f(dat[i * 2 + 1], dat[i * 2 + 2]);
        }

        return;
    }

    T query(int a, int b) {
        return query_sub(a, b, 0, 0, n);
    }

    T query_sub(int a, int b, int k, int l, int r) {
        if(b <= l || r <= a) {
            return E;
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            T vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return f(vl, vr);
        }
    }

    inline T operator[](const int i) {
        return query(i, i+1);
    }

    void print() {
        cout << "{";
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n-1) cout << ", ";
        }
        cout << "}";
        cout << "\n";
    }
};

auto f = [](TODO, TODO) -> TODO {
    TODO
};

// auto e = []() -> TODO {
//     TODO
// }



T f(T a, T b) {}
T E() {}

template<typename T>
class Abstruct_SegmentTree {
public:
    vector<T> dat;
    int n;

    Abstruct_SegmentTree(int n_) : dat(4*n_, E()) {
        int x = 1;
        while(x < n_) x *= 2;
        n = x;
    }

    void update(int i, T x) {
        i += n - 1;
        dat[i] = x;

        while(i > 0) {
            i = (i - 1) / 2;
            dat[i] = f(dat[i * 2 + 1], dat[i * 2 + 2]);
        }

        return;
    }

    T query(int a, int b) {
        return query_sub(a, b, 0, 0, n);
    }

    T query_sub(int a, int b, int k, int l, int r) {
        if(b <= l || r <= a) {
            return E();
        }else if(a <= l && r <= b) {
            return dat[k];
        }else {
            T vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return f(vl, vr);
        }
    }

    inline T operator[](const int i) {
        return query(i, i+1);
    }

    void print() {
        cout << "{";
        for(int i = 0; i < n; i++) {
            cout << (*this)[i];
            if(i != n-1) cout << ", ";
        }
        cout << "}";
        cout << "\n";
    }
};