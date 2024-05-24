//ref: https://null-mn.hatenablog.com/entry/2020/04/14/124151
template <typename T>
struct ReRooting {
    using F = function<T(T, int)>;
    using F2 = function<T(T, T)>;
    int V;
    vector<vector<int>> G;
    vector<vector<T>> dp;
    // dp_v = g(merge(f(dp_c1,c1), f(dp_c2,c2), ..., f(dp_ck,ck)), v)
    F f, g;
    F2 merge;
    T mi;  // identity of merge

    ReRooting() {}
    ReRooting(int V, F f, F2 merge, T mi, F g) : V(V), f(f), merge(merge), mi(mi), g(g) {
        G.resize(V);
        dp.resize(V);
    }

    void read(int idx = 1) {
        int a, b;
        for (int i = 0; i < V - 1; ++i) {
            cin >> a >> b;
            a -= idx, b -= idx;
            G[a].emplace_back(b);
            G[b].emplace_back(a);
        }
    }

    void add_edge(int a, int b) {
        G[a].emplace_back(b);
        G[b].emplace_back(a);
    }

    T dfs1(int p, int v) {
        T res = mi;
        for (int i = 0; i < G[v].size(); ++i) {
            if (G[v][i] == p) continue;
            dp[v][i] = dfs1(v, G[v][i]);
            res = merge(res, f(dp[v][i], G[v][i]));
        }
        return g(res, v);
    }

    void dfs2(int p, int v, T from_par) {
        for (int i = 0; i < G[v].size(); ++i) {
            if (G[v][i] == p) {
                dp[v][i] = from_par;
                break;
            }
        }
        vector<T> pR(G[v].size() + 1);
        pR[G[v].size()] = mi;
        for (int i = G[v].size(); i > 0; --i)
            pR[i - 1] = merge(pR[i], f(dp[v][i - 1], G[v][i - 1]));
        T pL = mi;
        for (int i = 0; i < G[v].size(); ++i) {
            if (G[v][i] != p) {
                T val = merge(pL, pR[i + 1]);
                dfs2(v, G[v][i], g(val, v));
            }
            pL = merge(pL, f(dp[v][i], G[v][i]));
        }
    }

    void calc(int root = 0) {
        for (int i = 0; i < V; ++i) dp[i].resize(G[i].size());
        dfs1(-1, root);
        dfs2(-1, root, mi);
    }

    T solve(int v) {
        T ans = mi;
        for (int i = 0; i < G[v].size(); ++i)
            ans = merge(ans, f(dp[v][i], G[v][i]));
        return g(ans, v);
    }
};