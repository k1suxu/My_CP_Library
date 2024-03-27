template<class S, S (*op)(S, S), S (*e)()>
struct SparseTable {
    vector<vector<S>> st;
    vector<int> lookup;

    SparseTable() = default;
    SparseTable(const vector<S> &v) {
        int b = 0;
        while((1 << b) <= v.size()) ++b;
        st.assign(b, vector<S>(1 << b));
        for(int i = 0; i < v.size(); i++) {
            st[0][i] = v[i];
        }
        for(int i = 1; i < b; i++) {
            for(int j = 0; j + (1 << i) <= (1 << b); j++) {
                st[i][j] = op(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
        lookup.resize(v.size() + 1);
        for(int i = 2; i < lookup.size(); i++) {
            lookup[i] = lookup[i >> 1] + 1;
        }
    }

    inline S query(int l, int r) {
        int b = lookup[r - l];
        return op(st[b][l], st[b][r - (1 << b)]);
    }
};

template<class S, S (*op)(S, S), S (*e)()>
struct Static_HeavyLightDecomposition {
    private:
    void dfs_sz(int cur) {
        size[cur] = 1;
        for (auto& dst : g[cur]) {
            if (dst == par[cur]) {
                if (g[cur].size() >= 2 && dst == g[cur][0])
                swap(g[cur][0], g[cur][1]);
                else
                continue;
            }
            depth[dst] = depth[cur] + 1;
            par[dst] = cur;
            dfs_sz(dst);
            size[cur] += size[dst];
            if (size[dst] > size[g[cur][0]]) {
                swap(dst, g[cur][0]);
            }
        }
    }

    void dfs_hld(int cur) {
        down[cur] = id++;
        for (auto dst : g[cur]) {
            if (dst == par[cur]) continue;
            nxt[dst] = (dst == g[cur][0] ? nxt[cur] : dst);
            dfs_hld(dst);
        }
        up[cur] = id;
    }

    // [u, v)
    vector<pair<int, int>> ascend(int u, int v) const {
        vector<pair<int, int>> res;
        while (nxt[u] != nxt[v]) {
            res.emplace_back(down[u], down[nxt[u]]);
            u = par[nxt[u]];
        }
        if (u != v) res.emplace_back(down[u], down[v] + 1);
        return res;
    }

    // (u, v]
    vector<pair<int, int>> descend(int u, int v) const {
        if (u == v) return {};
        if (nxt[u] == nxt[v]) return {{down[u] + 1, down[v]}};
        auto res = descend(u, par[nxt[v]]);
        res.emplace_back(down[nxt[v]], down[v]);
        return res;
    }

public:
    using Graph = vector<vector<int>>;

    Graph g;
    int n, id;
    vector<int> size, depth, down, up, nxt, par;
    vector<S> weights;
    SparseTable<S, op, e> table;

    Static_HeavyLightDecomposition() = default;
    Static_HeavyLightDecomposition(int n) : n(n), g(n), weights(n) {}
    Static_HeavyLightDecomposition(int n, vector<S> vertex_weight) : n(n), g(n), weights(vertex_weight) {assert(vertex_weight.size() == n);}
    //rebuildが呼ばれているのと等価
    Static_HeavyLightDecomposition(Graph _g, int root, vector<S> vertex_weight) :n(_g.size()), g(_g), id(0), size(g.size(), 0), depth(g.size(), 0), down(g.size(), -1), up(g.size(), -1), nxt(g.size(), root), par(g.size(), root), weights(vertex_weight) {
        dfs_sz(root);
        dfs_hld(root);
        vector<S> table_vector(n, e());
        for(int i = 0; i < n; i++) {
            table_vector[idx(i).first] = weights[i];
        }
        table = SparseTable<S, op, e>(table_vector);
    }

    //rebuildしないと更新は反映されない
    void vertex_set(int u, S val) {
        weights[u] = val;
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void rebuild(int root) {
        id = 0;
        size.assign(n, 0);
        depth.assign(n, 0);
        down.assign(n, -1);
        up.assign(n, -1);
        nxt.assign(n, root);
        par.assign(n, root);
        dfs_sz(root);
        dfs_hld(root);
        vector<S> table_vector(n);
        for(int i = 0; i < n; i++) table_vector[idx(i).first] = weights[i];
        table = SparseTable<S, op, e>(table_vector);
    }

    pair<int, int> idx(int i) const { return make_pair(down[i], up[i]); }

    //以降はrebuildまたはconstructorでbuildのどちらかが行われていないと正常に作動しない。
    S path_query(int u, int v) {
        S ans(e());
        int l = lca(u, v);
        for (pair<int, int> elem : ascend(u, l)) {
            int s = elem.first + 1, t = elem.second;
            ans = op(ans, s > t ? table.query(t, s) : table.query(s, t));
        }
        ans = op(ans, table.query(down[l], down[l] + 1));
        for (pair<int, int> elem : descend(l, v)) {
            int s = elem.first, t = elem.second + 1;
            ans = op(ans, s > t ? table.query(t, s) : table.query(s, t));
        }
        return ans;
    }

    S subtree_query(int u, bool include = true) {
        return table.query(down[u] + !include, up[u]);
    }

    int lca(int a, int b) {
        while (nxt[a] != nxt[b]) {
            if (down[a] < down[b]) swap(a, b);
            a = par[nxt[a]];
        }
        return depth[a] < depth[b] ? a : b;
    }

    int dist(int a, int b) { return depth[a] + depth[b] - depth[lca(a, b)] * 2; }
};

template<class S, S (*op)(S, S), S (*e)()>
using HLD = Static_HeavyLightDecomposition<S, op, e>;

using S = int;
S op(S x, S y) {
    return x|y;
}
S e() {
    return 0;
}

//disjoint sparse table ver.も作りたい