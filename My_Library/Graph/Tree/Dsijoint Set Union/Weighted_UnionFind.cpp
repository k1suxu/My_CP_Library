//Graph
//Tree
//Disjoint Set Union
//Weighted_UnionFind

struct Weighted_UnionFind {
    vector<int> r;
    vector<int> diff_weight;

    Weighted_UnionFind(int n) {
        r = vector<int>(n, -1);
        diff_weight = vector<int>(n, 0);
    }

    int root(int x) {
        if(r[x] < 0) return x;
        int R = root(r[x]);
        diff_weight[x] += diff_weight[r[x]];
        return r[x] = R;
    }
    //weight[y] - weight[x] = w;
    void unite(int x, int y, int w) {
        w += weight(x);
        w -= weight(y);
        x = root(x);
        y = root(y);
        if(x == y) return;
        //xの方が次数大きく
        if(r[x] < r[y]) swap(x, y), w = -w;
        r[y] = x;
        diff_weight[y] = w;
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    int weight(int x) {
        root(x);
        return diff_weight[x];
    }
    //return diff(x, y) if x, y stand in the same tree
    //返り値がINF -> 図れない(自由)
    int diff(int x, int y) {
        if(!issame(x, y)) return INF;
        return weight(y) - weight(x);
    }
};