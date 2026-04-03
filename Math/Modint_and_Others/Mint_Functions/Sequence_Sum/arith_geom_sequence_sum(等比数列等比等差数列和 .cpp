template<typename T, typename U = long long>
T x_geom_seq_sum(T r, U n) { // Σ [0,n) x^i
    if (n == 0) return T(0);
    if (n == 1) return T(1);

    if (n & 1) return T(1) + r * x_geom_seq_sum(r, n - 1);
    return x_geom_seq_sum(r * r, n / 2) * (T(1) + r);
}

// { r^n,  Σ [0,n) r^i,  Σ [0,n) i*r^i } を O(log n) で同時に計算する
template<typename T, typename U = long long>
std::tuple<T, T, T> ix_geom_seq_tuple(T r, U n) {
    if (n == 0) return {T(1), T(0), T(0)};
    
    U m = n / 2;
    auto [p_m, s_m, is_m] = ix_geom_seq_tuple(r, m);
    
    T p_2m = p_m * p_m;
    T s_2m = s_m + p_m * s_m;
    // Σ[0, 2m) i*r^i = Σ[0, m) i*r^i + r^m * Σ[0, m) (i+m)*r^i
    T is_2m = is_m + p_m * is_m + T(m) * p_m * s_m;
    
    if (n & 1) {
        T p_n = p_2m * r;
        T s_n = s_2m + p_2m;
        T is_n = is_2m + T(n - 1) * p_2m;
        return {p_n, s_n, is_n};
    }
    
    return {p_2m, s_2m, is_2m};
}

template<typename T, typename U = long long>
T ix_geom_seq_sum(T r, U n) { // Σ [0,n) ix^i
    return std::get<2>(ix_geom_seq_tuple(r, n));
}

// 初項a公比r長さnの等比数列の和を求める。つまり以下式を求める
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