template<class S, void (*merge)(S&, S)>
struct Undo_UnionFind_Component_Sum {
    stack<pair<int, S>> history;
    vector<S> r;

    Undo_UnionFind_Component_Sum() = default;
    Undo_UnionFind_Component_Sum(vector<S> init) : r(init) {}

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