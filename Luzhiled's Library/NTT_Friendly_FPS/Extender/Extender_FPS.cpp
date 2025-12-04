// Π(1 + a_ix)みたいなmulti linear productionも計算できる
// calc Π poly[i] with `devide and conquor algorithm`
// 次数の総和をNとしたときにO(N\log^2{N})
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> many_polynomials_production(vector<Inner_FPS<Mint>> poly) {
    const int N = (int)poly.size();
    if(N == 0) return {1};
    auto rec = [&](auto self, int l, int r) -> Inner_FPS<Mint> {
        if(l == r) return poly[l];
        int mid = (l+r)/2;
        return move(self(self, l, mid)) * move(self(self, mid+1, r));
    };
    return rec(rec, 0, N-1);
}

// インスタンスは自動解釈される
// ref: https://37zigen.com/multipoint-evaluation/
// used for `multipoint_evaluation` and `polynomial_interpolation` as a `sub` function
template<template<typename> class Inner_FPS, typename Mint>
vector<Inner_FPS<Mint>> subproduct_tree(const Inner_FPS<Mint> &xs) {
    const int n = (int)xs.size();
    int k = 1;
    while(k < n) k <<= 1;
    vector<Inner_FPS<Mint>> g(2*k, {1});
    for(int i = 0; i < n; i++) g[k+i] = {-xs[i], Mint(1)};
    for(int i = k; i-- > 1;) g[i] = g[i<<1] * g[i<<1 | 1];
    return g;
}

// インスタンスは自動解釈される
// N次多項式f, M個の評価点x1,x2,...,xmについてf(x1), f(x2), ..., f(xm)
// をO(N\log{N} + M\log^2{M})で求める
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> multipoint_evaluation(const Inner_FPS<Mint> &f, const Inner_FPS<Mint> &xs) {
    vector<Inner_FPS<Mint>> g = subproduct_tree(xs);
    int m = (int)xs.size(), k = (int)g.size() / 2;
    g[1] = f % g[1];
    for(int i = 2; i < k+m; i++) g[i] = g[i>>1] % g[i];
    Inner_FPS<Mint> ys(m);
    for(int i = 0; i < m; i++)  ys[i] = (g[k+i].empty()? Mint(0) : g[k+i][0]);
    return ys;
}

// インスタンスは自動解釈される
// for all i(0,2,...,N-1) f(xi)≡f(yi) をみたすN-1次多項式を求める(これは一意に定まる)
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> polynomial_interpolation(const Inner_FPS<Mint> &xs, const Inner_FPS<Mint> &ys) {
    assert(xs.size() == ys.size());
    vector<Inner_FPS<Mint>> mul = subproduct_tree(xs);
    int n = (int)xs.size(), k = (int)mul.size() / 2;
    vector<Inner_FPS<Mint>> g(2*k);
    g[1] = mul[1].diff() % mul[1];
    for(int i = 2; i < k+n; i++) g[i] = g[i>>1]%mul[i];
    for(int i = 0; i < n; i++) g[k+i] = {ys[i] / g[k+i][0]};
    for(int i = k; i-- > 1;) g[i] = g[i<<1] * mul[i<<1 | 1] + g[i<<1 | 1] * mul[i<<1];
    return g[1];
}

// インスタンスは自動解釈される
// Σf^i [0, n)を逆元なしで求める。
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> geometric_polynomial(const Inner_FPS<Mint> &x, const long long &n, const int &deg) {
    if (n == 0) return Inner_FPS<Mint>(deg, 0);

    long long m = n/2;
    Inner_FPS<Mint> half = geometric_polynomial(x, m, deg);
    Inner_FPS<Mint> res = half + x.pow(m, deg) * half;
    if (n % 2 == 1) {
        res *= x;
        ++res[0];
    }

    return res.pre(deg);
}

// Π(1 +- x ^ a_i)
// many_binomials_production<FPS, mint>
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> many_binomials_production(vector<int> a, int deg, bool neg) {
    vector<int> cnt(deg);
    for (auto &e : a) if (e < deg) cnt[e]++;
    Inner_FPS<Mint> f(deg);
    for (int i = 1; i < deg; i++) {
        Mint fac = 1;
        for (int j = i, t = 1; j < deg; j += i, t++) {
            if (neg) {
                f[j] -= Mint(cnt[i]) / Mint(t);
            } else {
                f[j] += fac * Mint(cnt[i]) / Mint(t);
            }
            fac *= -1;
        }
    }
    return f.exp(deg);
}