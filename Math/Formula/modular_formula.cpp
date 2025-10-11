long long inv(long long a, long long m) {
    long long b = m, u = 1, v = 0;
    while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    return (u + m) % m;
}

// ax ≡ b (mod m) を満たす最小の非負整数 x と解の間隔 d を求める
// 解なしの場合は {-1, -1} を返す
pair<long long, long long> modular_formula(long long a, long long b, long long m) {
    a = (a % m + m) % m;
    b = (b % m + m) % m;
    long long g = gcd(a, gcd(b, m));
    a /= g;
    b /= g;
    m /= g;
    if (gcd(a, m) != 1) return {-1, -1};
    long long x0 = (inv(a, m) * b % m + m) % m;
    long long d = m;
    return {x0, d};
}

// a, b, mのいずれかが0の場合の例外処理