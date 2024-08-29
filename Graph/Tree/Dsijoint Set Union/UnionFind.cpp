struct UnionFind {
    vector<int> r;
    
    UnionFind() = default;
    UnionFind(int n) {
        r = vector<int>(n, -1);
    }

    int root(int x) {
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

    bool all_connected() {
        return size(0) == (int)r.size();
    }

    // int number_of_set() {
    //     unordered_set<int> st;
    //     for(int i = 0; i < (int)r.size(); i++) st.insert(root(i));
    //     return st.size();
    // }

    // only vertices: not including leader pos
    // vector<vector<int>> decompose() {
    //     vector<pair<int, int>> p;
    //     p.reserve((int)r.size());
    //     for(int i = 0; i < (int)r.size(); i++) p.emplace_back(root(i), i);
    //     sort(all(p));
    //     //first:root, second:vertices
    //     vector<vector<int>> ret;
    //     int pre = -1;
    //     for(pair<int, int> e : p) {
    //         if(pre != e.first) {
    //             ret.push_back(vector<int>{e.second});
    //         }else {
    //             ret.back().push_back(e.second);
    //         }
    //         pre = e.first;
    //     }

    //     return ret;
    // }

    //leader and groups vertex
    // vector<pair<int, vector<int>>> decompose() {
    //     vector<pair<int, int>> p;
    //     for(int i = 0; i < (int)r.size(); i++) p.emplace_back(root(i), i);
    //     sort(all(p));
    //     //first:root, second:vertices
    //     vector<pair<int, vector<int>>> ret;
    //     int pre = -1;
    //     for(pair<int, int> e : p) {
    //         if(pre != e.first) {
    //             ret.push_back(make_pair(e.first, vector<int>{e.second}));
    //         }else {
    //             ret.back().second.push_back(e.second);
    //         }
    //         pre = e.first;
    //     }

    //     return ret;
    // }

    // vector<int> roots(vector<int> x) {
    //     vector<int> ret((int)x.size());
    //     for(int i = 0; i < (int)x.size(); i++) ret[i] = root(i);
    //     return ret;
    // }

    // bool unite(pair<int, int> p) {
    //     return unite(p.first, p.second);
    // }
    // vector<bool> unite(vector<pair<int, int>> p) {
    //     vector<bool> ret((int)p.size());
    //     for(int i = 0; i < (int)p.size(); i++) {
    //         ret[i] = unite(p[i]);
    //     }
    //     return ret;
    // }
    // vector<bool> unite(vector<int> x, vector<int> y) {
    //     assert(x.size() == y.size());
    //     vector<bool> ret((int)x.size());
    //     for(int i = 0; i < (int)x.size(); i++) {
    //         ret[i] = unite(x[i], y[i]);
    //     }
    //     return ret;
    // }

    // bool issame(pair<int, int> p) {
    //     return issame(p.first, p.second);
    // }
};