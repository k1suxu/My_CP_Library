//vector<long long> a(n);
//strongly increasing
int lis(const vector<long long> &a) {
    int n = (int)a.size();
    vector<long long> dp(n, INF);

    for(int i = 0; i < n; i++) {
        int id = (int)(lower_bound(all(dp), a[i]) - dp.begin());
        dp[id] = a[i];
    }

    return (int)(lower_bound(all(dp), INF) - dp.begin());
}

vector<long long> lis(const vector<long long> &a) {
    int n = (int)a.size();
    vector<long long> vec;
    for(int i = 0; i < n; i++) {
        auto it = lower_bound(all(vec), a[i]);
        if(it == vec.end()) vec.push_back(a[i]);
        else *it = a[i];
    }
    return vec;
}

//weakly increasing
vector<long long> lis(const vector<long long> &a) {
    int n = (int)a.size();
    vector<long long> vec;
    for(int i = 0; i < n; i++) {
        auto it = upper_bound(all(vec), a[i]);
        if(it == vec.end()) vec.push_back(a[i]);
        else *it = a[i];
    }
    return vec;
}

//strongly decreasing
vector<long long> lds(vector<long long> a) {
    int n = (int)a.size();
    for(int i = 0; i < n; i++) a[i] = -a[i];
    vector<long long> vec;
    for(int i = 0; i < n; i++) {
        auto it = lower_bound(all(vec), a[i]);
        if(it == vec.end()) vec.push_back(a[i]);
        else *it = a[i];
    }
    for(int i = 0; i < (int)vec.size(); i++) {
        vec[i] = -vec[i];
    }
    reverse(all(vec));
    return vec;
}

//weakly decreasing
vector<long long> lds(vector<long long> a) {
    int n = (int)a.size();
    for(int i = 0; i < n; i++) a[i] = -a[i];
    vector<long long> vec;
    for(int i = 0; i < n; i++) {
        auto it = upper_bound(all(vec), a[i]);
        if(it == vec.end()) vec.push_back(a[i]);
        else *it = a[i];
    }
    for(int i = 0; i < (int)vec.size(); i++) {
        vec[i] = -vec[i];
    }
    reverse(all(vec));
    return vec;
}