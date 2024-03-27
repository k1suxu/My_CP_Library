//Graph
//Tree
//Disjoint Set Union
//Weighted_UnionFind

template<typename T_Weight>
struct Weighted_UnionFind {
    vector<int> r;
    vector<T_Weight> diff_weight;

    Weighted_UnionFind(int n) {
        r = vector<int>(n, -1);
        diff_weight = vector<T_Weight>(n, 0);
    }

    int root(int x) {
        if(r[x] < 0) return x;
        int R = root(r[x]);
        diff_weight[x] += diff_weight[r[x]];
        return r[x] = R;
    }
    //weight[y] - weight[x] = w;
    void unite(int x, int y, T_Weight w) {
        w += weight(x);
        w -= weight(y);
        x = root(x);
        y = root(y);
        if(x == y) return;
        
        if(r[x] < r[y]) swap(x, y), w = -w;
        r[y] = x;
        diff_weight[y] = w;
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    T_Weight weight(int x) {
        root(x);
        return diff_weight[x];
    }
    //return diff(x, y) if x, y stand in the same tree
    //weight[y] - weight[x]
    T_Weight diff(int x, int y) {
        if(!issame(x, y)) return INF;
        return weight(y) - weight(x);
    }
};

//[TODO]: 矛盾を返すようにしたい