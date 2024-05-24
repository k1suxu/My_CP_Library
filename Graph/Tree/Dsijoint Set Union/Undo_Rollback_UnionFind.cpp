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
            r[history.back().first] = history.back().second;
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

//not verifed
//https://nyaannyaan.github.io/library/data-structure/rollback-union-find.hpp.html
//みたいにstateを引数にするのもあり。また、hisotryをstackにしたい