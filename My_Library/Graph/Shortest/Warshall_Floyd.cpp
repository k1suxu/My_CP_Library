//GRAPH
// warshall_floyd
// if dp[i][i] < 0 -> it has negativecycle
// O(N^3)
vector<vector<int>> warshall_floyd(vector<vector<pair<int, int>>> &g) {
    int n = g.size();
    vector<vector<int>> dp(n, vector<int>(n, INF));
    for(int i = 0; i < n; i++) {
        for(auto e : g[i]) {
            dp[i][e.first] = e.second;
        }
    }
    for(int i = 0; i < n; i++) {
        dp[i][i] = 0;
    }
    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(dp[i][k] == INF || dp[k][j] == INF) continue;
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
            }
        }
    }
    return dp;
}
