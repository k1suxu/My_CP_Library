struct edge {
    int from, to, cost;

    edge() = default;
    edge(int f, int t, int c) : from(f), to(t), cost(c) {}

    bool operator<(const edge other) const {
        return cost < other.cost;
    }
    bool operator>(const edge other) const {
        return cost > other.cost;
    }
};

struct UnionFind {
    vector<int> r;

    UnionFind(int n) : r(n, -1) {}

    int root(int x) {
        if(r[x] < 0) return x;
        return r[x] = root(r[x]);
    }

    void unite(int x, int y) {
        x = root(x);
        y = root(y);
        if(x == y) return;
        if(r[x] > r[y]) swap(x, y);
        r[x] += r[y];
        r[y] = x;
        return;
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    int size(int i) {
        return -r[root(i)];
    }
};

int kruskal(int n, vector<edge> edges) {
    sort(edges.begin(), edges.end());
    UnionFind UF(n);
    int ans = 0;

    for(auto e : edges) {
        if(!UF.issame(e.from, e.to)) {
            UF.unite(e.from, e.to);
            ans += e.cost;
        }
    }

    return ans;
}