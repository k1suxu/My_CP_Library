template<typename T, typename U = long long>
T x_geom_seq_sum(T r, U n) { // Σ [0,n) x^i
    if(n == 0) return 0;

    U m = n/2;
    T half = x_geom_seq_sum(r, m);
    T ret = half + r.pow(m) * half;
    if(n&1) ret = 1 + ret * r;

    return ret;
}

template<typename T, typename U = long long>
T ix_geom_seq_sum(T r, U n) { // Σ [1,n) ix^i (= Σ [0,n) ix^i)
    if(n == 0) return 0;
    
    U m = n/2;
    T half = ix_geom_seq_sum(r, m);
    T ret = half + r.pow(m) * half;
    ret += m * r.pow(m) * x_geom_seq_sum(r, m);
    if(n&1) ret = ret * r + x_geom_seq_sum(r, n) - 1;

    return ret;
}

// 初項1公比r長さnの等比数列の和を求める。つまり以下式を求める
// a + ar + ar^2 + ... + r^(n-1) = Σ [0,n) ar^i
// O(log^2 n)
template<typename T, typename U = long long>
T geom_seq_sum(T a, T r, U n) {
    return a * x_geom_seq_sum(r, n);
}
// 等差数列の初項a,公差d,等比数列の初項b,公比r、長さnの(等差)×(等比)型の数列の和を求める
// つまり以下式を求める
// ab + (a+d)br + (a+2d)br^2 + ... + (a+d(n-1))br^(n-1) = b Σ [0,n) (a+di)r^i
// = b (a*(Σ [0,n) r^i) + d*(Σ [0,n) ir^i))   // O(log^2 n)
template<typename T, typename U = long long>
T arith_geom_seq_sum(T a, T d, T b, T r, U n) {
    return b * (a * x_geom_seq_sum(r, n) + d * ix_geom_seq_sum(r, n));
}