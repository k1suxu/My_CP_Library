//ref: https://atcoder.jp/contests/abc133/submissions/me
template<typename T = int>
class Weighted_LCA{
    private:
    int n;
    int root;
    int k;
    vector<vector<pair<int, T>>> _G;
    vector<vector<int>> dp;
    vector<T> dist;
    vector<int> depth;

    public:
    Weighted_LCA() = default;
    Weighted_LCA(const int N) : n(N), _G(N) {}
    Weighted_LCA(const int IGNORE, const vector<vector<pair<int, T>>> G) : n((int)G.size()), _G(G) {}

    void add_edge(int u, int v, T w) {
        _G[u].emplace_back(v, w);
        _G[v].emplace_back(u, w);
        return;
    }
    
    void rebuild(const int _root = 0) {
        root = _root;
        k = 1;
        int two = 2;
        while(two < n) {
            two <<= 1;
            k++;
        }
        dist = vector<T>(n, INF);
        dist[_root] = 0;
        dp = vector<vector<int>>(k+1, vector<int>(n, -1));
        depth.resize(n);
        function<void(int, int)> _dfs = [&](int v, int p) {
            dp[0][v] = p;
            for(auto nv : _G[v]) {
                if(nv.first == p) continue;
                depth[nv.first] = depth[v] + 1;
                dist[nv.first] = dist[v] + nv.second;
                _dfs(nv.first, v);
            }
        };
        _dfs(root, -1);

        for(int i = 0; i < k; i++) {
            for(int j = 0; j < n; j++) {
                if(dp[i][j] == -1) continue;
                dp[i + 1][j] = dp[i][dp[i][j]];
            }
        }
    }

    int get(int u, int v) {
        if(depth[u] < depth[v]) swap(u, v);
        if(depth[u] != depth[v]) {
            long long d = depth[u] - depth[v];
            for(int i = 0; i < k; i++) if((d>>i) & 1) u = dp[i][u];
        }

        if(u==v) return u;

        //一致する点のひとつ前のところに移動。
        for(int i = k; i >= 0; i--) {
            if(dp[i][u] != dp[i][v]) {
                u = dp[i][u];
                v = dp[i][v];
            }
        }
        return dp[0][u];
    }

    int get_node_distance(const int u, const int v) {
        int lca = get(u, v);
        return depth[u] + depth[v] - 2*depth[lca];
    }
    int get_weight_distance(const int u, const int v) {
        int lca = get(u, v);
        return dist[u] + dist[v] - 2*dist[lca];
    }
    int get_kth_ancestor(int u, const int K) {
        for(int i = 0; i < k; i++) {
            if(K >> i & 1) u = dp[i][u];
        }
        return u;
    }
    pair<int, int> get_distance_to_LCA(const int u, const int v) {
        int lca = get(u, v);
        return make_pair(dist[u] - dist[lca], dist[v] - dist[lca]);
    }
};

// template<typename C=int, typename D=int>
struct colored_edge_path_qeyry {
    struct edge {
        int to;
        int color;
        int dist;
        edge() = default;
        edge(int t, int c, int d) : to(t), color(c), dist(d) {}
    };

    int n;
    vector<vector<edge>> g;
    Weighted_LCA<int> lca;
    vector<vector<int>> color_tot;
    vector<vector<int>> color_num;
    vector<vector<int>> color_ids;
    vector<int> in, out;
    int order;

    colored_edge_path_qeyry() = default;
    colored_edge_path_qeyry(const int Vertex_Num) : n(Vertex_Num) {
        g = vector<vector<edge>>(Vertex_Num);
        lca = Weighted_LCA<int>(Vertex_Num);
    }

    void add_edge(int from, int to, int color, int dist) {
        g[from].emplace_back(to, color, dist);
        g[to].emplace_back(from, color, dist);
        lca.add_edge(from, to, dist);
    }

    void dfs(int v, int p) {
        in[v] = order++;
        for(auto e : g[v]) if(e.to != p) {
            color_tot[e.color].push_back(e.dist);
            color_ids[e.color].push_back(order++);
            color_num[e.color].push_back(1);
            dfs(e.to, v);
            color_tot[e.color].push_back(-e.dist);
            color_ids[e.color].push_back(order++);
            color_num[e.color].push_back(-1);
        }
        out[v] = order++;
    };

    void rebuild(const int root, const int Color_Limit) {
        color_tot = vector<vector<int>>(Color_Limit, {0});
        color_num = vector<vector<int>>(Color_Limit, {0});
        color_ids = vector<vector<int>>(Color_Limit);
        in = vector<int>(n);
        out = vector<int>(n);
        order = 0;
        lca.rebuild(root);

        dfs(0, -1);

        for(auto& e : color_tot) {
            FOR(e.size()-1) {
                e[i+1] += e[i];
            }
        }
        for(auto& e : color_num) {
            FOR(e.size() - 1) {
                e[i+1] += e[i];
            }
        }
    }

    int query(int color, int new_dist, int u, int v) {
        int ancestor = lca.get(u, v);

        auto center = (int)(lower_bound(all(color_ids[color]), in[ancestor]) - color_ids[color].begin());
        auto it1 = (int)(lower_bound(all(color_ids[color]), in[u]) - color_ids[color].begin());
        auto it2 = (int)(lower_bound(all(color_ids[color]), in[v]) - color_ids[color].begin());

        int ans = lca.get_weight_distance(u, v);
        ans -= color_tot[color][it1] + color_tot[color][it2] - 2 * color_tot[color][center];
        ans += new_dist * (color_num[color][it1] + color_num[color][it2] - 2 * color_num[color][center]);

        return ans;
    }
};