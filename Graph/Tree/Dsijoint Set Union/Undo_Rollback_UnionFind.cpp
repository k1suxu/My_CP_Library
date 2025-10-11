struct Undo_UnionFind {
    stack<pair<int, int>> history;
    vector<int> r;

    Undo_UnionFind() = default;
    Undo_UnionFind(int n) : r(n, -1) {}

    //ただ単につないでmerge
    bool unite(int x, int y) {
        x = root(x);
        history.emplace(x, r[x]);
        y = root(y);
        history.emplace(y, r[y]);
        if(x == y) return false;
        if(r[x] > r[y]) swap(x, y);
        r[x] += r[y];
        r[y] = x;
        return true;
    }

    int root(int x) {
        if(r[x] < 0) return x;
        return root(r[x]);
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }
    
    int size(int x) {
        return -r[root(x)];
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
        while(!history.empty()) history.pop();
    }

    void rollback() {
        while(!history.empty()) undo();
    }
};