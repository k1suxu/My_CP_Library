int number_of_substrings(string s, bool empty_include = true) {
    int n = (int)s.size();

    vector<vector<int>> next_char(n+1, vector<int>(26, n));
    for(int i = n-1; i >= 0; i--) {
        for(int j = 0; j < 26; j++) next_char[i][j] = next_char[i+1][j];
        next_char[i][s[i] - 'a'] = i;
    }

    vector<int> dp(n+1, 0);
    dp[0] = 1;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < 26; j++) {
            if (next_char[i][j] >= n) continue;
            (dp[next_char[i][j]+1] += dp[i]) %= mod;
        }
    }

    int ret = (empty_include? 0 : -1);
    for(int i = 0; i <= n; i++) (ret += dp[i]) %= mod;

    return ret;
}