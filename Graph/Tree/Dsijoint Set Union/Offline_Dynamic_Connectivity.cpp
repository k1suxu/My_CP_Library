template<class S, void (*merge)(S&, S)>
struct Undo_UnionFind_Component_Sum {
    stack<pair<int, S>> history;
    vector<S> r;

    Undo_UnionFind_Component_Sum() = default;
    explicit Undo_UnionFind_Component_Sum(int n) : r(n, S()) {}
    explicit Undo_UnionFind_Component_Sum(vector<S> init) : r(init) {}

    //ただ単につないでmerge
    bool unite(int x, int y) {
        x = root(x);
        history.emplace(x, r[x]);
        y = root(y);
        history.emplace(y, r[y]);
        if(x == y) return false;
        if(r[x].to > r[y].to) swap(x, y);
        merge(r[x], r[y]);
        r[x].to += r[y].to;
        r[y].to = x;
        return true;
    }

    //つなぐしcomponentにaddする
    bool unite_add(int x, int y, S add) {
        x = root(x);
        history.emplace(x, r[x]);
        y = root(y);
        history.emplace(y, r[y]);
        if(x == y) {
            merge(r[x], add);
            return false;
        }
        if(r[x].to > r[y].to) swap(x, y);
        merge(r[x], r[y]);
        merge(r[x], add);
        r[x].to += r[y].to;
        r[y].to = x;
        return true;
    }
    // bool node_add(int x, S add) {
    //     return unite_add(x, x, add);
    // }

    int root(int x) {
        if(r[x].to < 0) return x;
        return root(r[x].to);
        // return r[x].to = root(r[x].to);
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }
    
    int size(int x) {
        return r[root(x)].size();
    }

    S aggregate(int x) {
        return r[root(x)];
    }

    bool undo() {
        if(history.size() == 0) return false;
        for(int i = 0; i < 2; ++i) {
            r[history.top().first] = history.top().second;
            history.pop();
        }
        return true;
    }

    void snapshot() {
        history.clear();
    }

    void rollback() {
        while(!history.empty()) undo();
    }
};

template<class S, void (*merge)(S&, S)>
struct Offline_Dynamic_Connectivity {
    using edge = pair< int, int >;

    Undo_UnionFind_Component_Sum<S, merge> uf;
    int V, Q, segsz;
    vector< vector< edge > > seg;
    int comp;

    vector< pair< pair< int, int >, edge > > pend;
    map< edge, int > cnt, appear;

    Offline_Dynamic_Connectivity(int V, int Q) : uf(V), V(V), Q(Q), comp(V) {
        segsz = 1;
        while(segsz < Q) segsz <<= 1;
        seg.resize(2 * segsz - 1);
    }

    void insert(int idx, int s, int t) {
        auto e = minmax(s, t);
        if(cnt[e]++ == 0) appear[e] = idx;
    }

    void erase(int idx, int s, int t) {
        auto e = minmax(s, t);
        if(--cnt[e] == 0) pend.emplace_back(make_pair(appear[e], idx), e);
    }

    void add(int a, int b, const edge &e, int k, int l, int r) {
        if(r <= a || b <= l) return;
        if(a <= l && r <= b) {
            seg[k].emplace_back(e);
            return;
        }
        add(a, b, e, 2 * k + 1, l, (l + r) >> 1);
        add(a, b, e, 2 * k + 2, (l + r) >> 1, r);
    }

    void add(int a, int b, const edge &e) {
        add(a, b, e, 0, 0, segsz);
    }

    void build() {
        for(auto &p : cnt) {
            if(p.second > 0) pend.emplace_back(make_pair(appear[p.first], Q), p.first);
        }
        for(auto &s : pend) {
            add(s.first.first, s.first.second, s.second);
        }
    }

    void run(const function< void(int) > &f, int k = 0) {
        int add = 0;
        for(auto &e : seg[k]) {
            add += uf.unite(e.first, e.second);
        }
        comp -= add;
        if(k < segsz - 1) {
            run(f, 2 * k + 1);
            run(f, 2 * k + 2);
        } else if(k - (segsz - 1) < Q) {
            int query_index = k - (segsz - 1);
            f(query_index);
        }
        for(auto &e : seg[k]) {
            uf.undo();
        }
        comp += add;
    }
};

//mergeの計算にto(size)が必要な可能性もあるのでSの中でtoを定義すること
struct S {
    int to;

    S() : to(-1) {}

    int size() {
        assert(to<0);
        return -to;
    }
};
void merge(S &par, S ch) { // toの更新は必要ない
}

// 同じ時刻(idxが等しい)にいくつ追加してもOK
// f(i) = UF.size(x[i]), UF.aggregate(x[i])とかでも対応可能！
// ref: https://atcoder.jp/contests/abc356/submissions/54138021