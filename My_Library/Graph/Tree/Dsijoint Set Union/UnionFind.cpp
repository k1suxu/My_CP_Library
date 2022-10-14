//普通UnionFind -- Disjoing Set Union
struct UnionFind {
    vector<int> r;
    
    UnionFind(int n) {
        r = vector<int>(n, -1);
    }

    int root(int x) {
        if(r[x] < 0) return x;
        return r[x] = root(r[x]);
    }

    bool unite(pair<int, int> p) {
        return unite(p.first, p.second);
    }

    bool unite(int x, int y) {
        x = root(x);
        y = root(y);
        if(x == y) return false;
        if(r[x] > r[y]) swap(x, y);
        r[x] += r[y];
        r[y] = x;
        return true;
    }

    bool issame(pair<int, int> p) {
        return issame(p.first, p.second);
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    int size(int x) {
        return -r[root(x)];
    }

    int number_of_set() {
        set<int> st;
        FOR(r.size()) st.insert(root(i));
        return st.size();
    }
};