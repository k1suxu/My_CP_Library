struct BigUnionFind {
    unordered_map<int, int> r;
    vector<int> vertices;

    BigUnionFind() = default;

    void insert(int x) {
        if(r.find(x) != r.end()) return;
        r[x] = -1;
        vertices.push_back(x);
        return;
    }

    void insert(vector<int> a) {
        for(int e : a) insert(e);
        return;
    }

    int root(int x) {
        assert(r.find(x) != r.end());
        if(r[x] < 0) return x;
        return r[x] = root(r[x]);
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

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    int size(int x) {
        return -r[root(x)];
    }

    int number_of_set() {
        set<int> st;
        for(int i = 0; i < (int)r.size(); i++) st.insert(root(i));
        return st.size();
    }

    vector<vector<int>> decompose() {
        vector<pair<int, int>> p;
        for(int i = 0; i < (int)r.size(); i++) p.emplace_back(root(i), i);
        sort(all(p));
        vector<vector<int>> ret;
        int pre = -1;
        for(pair<int, int> e : p) {
            if(pre != e.first) ret.back().push_back(e.second);
            else ret.push_back(vector<int>());
        }

        for(vector<int> &e : ret) sort(e.begin(), e.end());
        return ret;
    }

    vector<int> roots(vector<int> x) {
        vector<int> ret((int)x.size());
        for(int i = 0; i < (int)x.size(); i++) ret[i] = root(i);
        return ret;
    }

    bool unite(pair<int, int> p) {
        return unite(p.first, p.second);
    }
    vector<bool> unite(vector<pair<int, int>> p) {
        vector<bool> ret((int)p.size());
        for(int i = 0; i < (int)p.size(); i++) {
            ret[i] = unite(p[i]);
        }
        return ret;
    }
    vector<bool> unite(vector<int> x, vector<int> y) {
        assert(x.size() == y.size());
        vector<bool> ret((int)x.size());
        for(int i = 0; i < (int)x.size(); i++) {
            ret[i] = unite(x[i], y[i]);
        }
        return ret;
    }

    bool issame(pair<int, int> p) {
        return issame(p.first, p.second);
    }

    vector<int> get_vertices() {
        return vertices;
    }
};