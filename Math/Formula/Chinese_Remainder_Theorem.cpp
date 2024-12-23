long long extended_gcd(long long a, long long b, long long &x, long long &y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long d = extended_gcd(b, a%b, y, x);
    y -= a/b*x;
    return d;
}

//中国剰余定理 O(N log M)
//i=0,1,...,n-1 について　x ≡ b_i (mod m_i)　を満たすxを、x = r+mZとして返す
//first: r, second: m
//Π m が64bitに収まらない場合、オーバーフローする可能性あり
//合ってるかどうかO(N)で判定すること！！！
//for all i (1<=i<=N) r%(m[i]) == b[i]
pair<long long, long long> Chinese_Remainder_Theorem(const vector<long long> b, const vector<long long> m) {
    long long r = 0, M = 1;
    for(int i = 0; i < (int)b.size(); ++i) {
        long long p, q;
        long long d = extended_gcd(M, m[i], p, q);
        if((b[i] - r) % d != 0) return make_pair(0, -1);
        long long tmp = (b[i] - r) / d * p % (m[i]/d);
        r += M * tmp;
        M *= m[i]/d;
    }
    return make_pair((r%M+M)%M, M);
}