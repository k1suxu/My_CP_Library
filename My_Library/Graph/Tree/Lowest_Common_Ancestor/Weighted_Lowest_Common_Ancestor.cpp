//weight のタイプ
template<typename T = int>
class Weighted_LCA{
    private:
    int root;
    int k;
    vector<vector<int>> dp;
    vector<T> dist;
    vector<int> depth;

    public:
    Weighted_LCA(const vector<vector<pair<int, T>>>& _G, const int _root = 0) {
        int n = (int)_G.size();
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

//add_edge version
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