string LCS(string s, string t) {
    int n = (int)s.size();
    int m = (int)t.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(s[i] == t[j]) dp[i + 1][j + 1] = max({dp[i][j + 1], dp[i + 1][j], dp[i][j] + 1});
            else dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
        }
    }

    int len = dp[n][m];
    int i = n - 1;
    int j = m - 1;
    string ans(len, '#');
    while(len) {
        if(s[i] == t[j]) {
            ans[--len] = s[i];
            i--;
            j--;
        }else if(dp[i + 1][j + 1] == dp[i][j + 1]) {
            i--;
        }else {
            j--;
        }
    }

    return ans;
}

template<typename T>
vector<T> LCS(vector<T> s, vector<T> t) {
    int n = (int)s.size();
    int m = (int)t.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(s[i] == t[j]) dp[i + 1][j + 1] = max({dp[i][j + 1], dp[i + 1][j], dp[i][j] + 1});
            else dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
        }
    }

    int len = dp[n][m];
    int i = n - 1;
    int j = m - 1;
    vector<T> ans(len);
    while(len) {
        if(s[i] == t[j]) {
            ans[--len] = s[i];
            i--;
            j--;
        }else if(dp[i + 1][j + 1] == dp[i][j + 1]) {
            i--;
        }else {
            j--;
        }
    }

    return ans;
}