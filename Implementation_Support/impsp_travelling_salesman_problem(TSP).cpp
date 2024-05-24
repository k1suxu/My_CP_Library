signed main() {
    int n;
    cin >> n;
    vi x(n), y(n), z(n);
    FOR(n) cin >> x[i] >> y[i] >> z[i];

    auto dist = [&](int i, int j) -> int {
        return abs(x[i]-x[j])+abs(y[i]-y[j])+max(0LL, z[j]-z[i]);
    };

    vvi dp(1<<n, vi(n, INF));
    FOR(n) dp[0][i] = dist(0, i);

    rep(i, 1<<n) {
        rep(j, n) {
            if(i>>j&1) {
                rep(k, n) {
                    chmin(dp[i][j], dp[i^(1<<j)][k]+dist(k, j));
                }
            }
        }
    }

    cout << dp[(1<<n)-1][0] << endl;
}