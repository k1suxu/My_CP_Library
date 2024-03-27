vector<long long> f = {1}, rf = {1};
long long inv(long long x) {
    long long res = 1;
    long long k = mod - 2;
    long long y = x;
    while (k) {
        if (k & 1) res = (res * y) % mod;
        y = (y * y) % mod;
        k /= 2;
    }
    return res;
}
// void init(long long n) {
//     f.resize(n, 0);
//     rf.resize(n, 0);
//     f[0] = 1;
//     repi(i, 1, n) f[i] = (f[i - 1] * i) % mod;
//     repi(i, 0, n) rf[i] = inv(f[i]);
// }
//---------------------------------------------------------------------------------------------------
long long P(long long n, long long k) {
    while(n > f.size() - 1) {
        f.push_back(f.back() * f.size() % mod);
        rf.push_back(inv(f.back()));
    }
    long long a = f[n];
    long long b = rf[n - k];
    return a * b % mod;
}

long long C(long long n, long long k) {
    while(n > f.size() - 1) {
        f.push_back(f.back() * f.size() % mod);
        rf.push_back(inv(f.back()));
    }
    long long a = f[n]; // = n!
    long long b = rf[n-k]; // = (n-k)!
    long long c = rf[k]; // = k!

    long long bc = (b * c) % mod;

    return (a * bc) % mod;
}

long long H(long long n, long long k) {
    return C(n + k - 1, k);
}

//カタラン数
long long Cat(long long n) {
    long long ret = C(2*n, n)-C(2*n, n-1);
    return (ret % mod + mod) % mod;
}