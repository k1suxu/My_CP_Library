// 基本的には0^0=1
long long powmod(long long n, long long m, const long long MOD) {
    // // 0^0 = 0なら以下の行が必要
    // if(n == 0) return 0;
    long long res = 1;
    while(m > 0) {
        if(m&1) res = (res*n)%MOD;
        n = (n*n)%MOD;
        m >>= 1;
    }
    return res;
}

//a^x=b(mod p)を満たすxのうちlower以上の最小値を返す
//lower=0(未指定)なら非負整数
//存在しない場合は-1を返す
//0^0 = 1
long long discrete_log(long long a, long long b, long long p, long long lower = 0) {
    long long g = 1;
    
    for(long long i = p; i; i /= 2) (g *= a) %= p;
    g = __gcd(g, p);
    
    long long t = powmod(a, lower, p), c = 0;
    for(; t % g; c++) {
        if(t == b) return lower + c;
        (t *= a) %= p;
    }
    if(b % g) return -1;
    
    t /= g;
    b /= g;
    
    long long n = p / g, h = 0, gs = 1;
    
    for(; h * h < n; h++) (gs *= a) %= n;
    
    unordered_map<long long, long long> bs;
    for(long long s = 0, e = b; s < h; bs[e] = ++s) {
        (e *= a) %= n;
    }
    
    for(long long s = 0, e = t; s < n;) {
        (e *= gs) %= n;
        s += h;
        if(bs.count(e)) return lower + c + s - bs[e];
    }
    return -1;
}