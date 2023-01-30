struct UnionFind {
    vector<int> r;
    vector<int> lead;
    
    UnionFind(int n) {
        r = vector<int>(n, -1);
        lead.resize(n);
        iota(lead.begin(), lead.end(), 0);
    }
    UnionFind(int n, vector<int> prim_leader) {
        r = vector<int>(n, -1);
        lead = prim_leader;
    }

    int root(int x) {
        if(r[x] < 0) return x;
        return r[x] = root(r[x]);
    }

    //xのグループにyのグループをくっつける
    //leader[y] := leader[x]
    bool unite(int x, int y) {
        x = root(x);
        y = root(y);
        if(x == y) return false;
        lead[y] = lead[x];
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

    int leader(int x) {
        return lead[root(x)];
    }
    //xが入っているグループのleaderをyに変える(変える必要がない(同じだったらfalse))
    bool change_leader(int x, int y) {
        if(lead[root(x)] == y) return false;
        lead[root(x)] = y;
        return true;
    }
};