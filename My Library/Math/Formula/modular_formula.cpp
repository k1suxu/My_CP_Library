long long inv(long long n, long long m) {
    long long a = (n%m+m)%m, b = m, u = 1, v = 0;
    while(b) {
        long long t = a/b;
        a -= t*b, swap(a, b);
        u -= t*v, swap(u, v);
    }
    return u;
}
//ax≡b mod mを満たす最小のxを求める
//-1 : 解なし。
long long modular_formula(long long a, long long b, long long m) {
    long long d = __gcd(a, __gcd(b, m));
    a /= d;
    b /= d;
    m /= d;
    if(__gcd(a, m) != 1) return -1;
    return (inv(a, m) * b % m + m) % m;
}