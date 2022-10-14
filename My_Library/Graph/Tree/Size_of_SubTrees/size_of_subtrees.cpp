//include itsefl
vector<int> size_of_subtrees(vector<vector<int>> g, const int root = 0) {
    int n = (int)g.size();
    vector<int> dp(n, 0);

    auto dfs = [&](int v, int p, auto self) -> int {
        for(auto e : g[v]) if(e != p) {
            dp[v] += self(e, v, self);
        }
        return ++dp[v];
    };

    dfs(root, -1, dfs);

    return dp;
}

//not verified
vector<vector<int>> size_of_subtrees(vector<vector<int>> g, const int root = 0) {
    int n = (int)g.size();
    vector<vector<int>> ret(n);
    vector<int> dp(n, 0);

    auto dfs = [&](int v, int p, auto self) -> int {
        for(auto e : g[v]) if(e != p) {
            int now = self(e, v, self);
            ret[v].push_back(now);
            dp[v] += now;
        }

        ret[v].push_back(n-1-dp[v]);
        
        return ++dp[v];
    };

    dfs(root, -1, dfs);

    return ret;
}