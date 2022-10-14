vector<int> f = {1}, rf = {1};
ll inv(ll x) {
    ll res = 1;
    ll k = mod - 2;
    ll y = x;
    while (k) {
        if (k & 1) res = (res * y) % mod;
        y = (y * y) % mod;
        k /= 2;
    }
    return res;
}
// void init(int n) {
//     f.resize(n, 0);
//     rf.resize(n, 0);
//     f[0] = 1;
//     repi(i, 1, n) f[i] = (f[i - 1] * i) % mod;
//     repi(i, 0, n) rf[i] = inv(f[i]);
// }
//---------------------------------------------------------------------------------------------------
ll P(int n, int k) {
    while(n > f.size() - 1) {
        f.push_back(f.back() * f.size() % mod);
        rf.push_back(inv(f.back()));
    }
    ll a = f[n];
    ll b = rf[n - k];
    return a * b % mod;
}

ll C(int n, int k) {
    while(n > f.size() - 1) {
        f.push_back(f.back() * f.size() % mod);
        rf.push_back(inv(f.back()));
    }
    ll a = f[n]; // = n!
    ll b = rf[n-k]; // = (n-k)!
    ll c = rf[k]; // = k!

    ll bc = (b * c) % mod;

    return (a * bc) % mod;
}

ll H(int n, int k) {
    return C(n + k - 1, k);
}

//ÉJÉ^ÉâÉìêî
ll Cat(int n) {
    ll ret = C(2*n, n)-C(2*n, n-1);
    return (ret % mod + mod) % mod;
}