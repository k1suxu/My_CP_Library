// Π(1 + a_ix)みたいなmulti linear productionも計算できる
// calc Π poly[i] with `devide and conquor algorithm`
// 次数の総和をNとしたときにO(N\log^2{N})
template<typename Mint>
FPS<Mint> many_polynomials_production(vector<FPS<Mint>> poly) {
    const int N = (int)poly.size();
    if(N == 0) return {1};
    auto rec = [&](auto self, int l, int r) -> FPS<Mint> {
        if(l == r) return poly[l];
        int mid = (l+r)/2;
        return move(self(self, l, mid)) * move(self(self, mid+1, r));
    };
    return rec(rec, 0, N-1);
}

// インスタンスは自動解釈される
// ref: https://37zigen.com/multipoint-evaluation/
// used for `multipoint_evaluation` and `polynomial_interpolation` as a `sub` function
template<typename Mint>
vector<FPS<Mint>> subproduct_tree(const vector<Mint> &xs) {
    const int n = (int)xs.size();
    int k = 1;
    while(k < n) k <<= 1;
    vector<FPS<Mint>> g(2*k, {1});
    for(int i = 0; i < n; i++) g[k+i] = {-xs[i], Mint(1)};
    for(int i = k; i-- > 1;) g[i] = g[i<<1] * g[i<<1 | 1];
    return g;
}

// インスタンスは自動解釈される
// N次多項式f, M個の評価点x1,x2,...,xmについてf(x1), f(x2), ..., f(xm)
// をO(N\log{N} + M\log^2{M})で求める
template<typename Mint>
vector<Mint> multipoint_evaluation(const FPS<Mint> &f, const vector<Mint> &xs) {
    vector<FPS<Mint>> g = subproduct_tree(xs);
    int m = (int)xs.size(), k = (int)g.size() / 2;
    g[1] = f % g[1];
    for(int i = 2; i < k+m; i++) g[i] = g[i>>1] % g[i];
    vector<Mint> ys(m);
    for(int i = 0; i < m; i++)  ys[i] = (g[k+i].empty()? Mint(0) : g[k+i][0]);
    return ys;
}

// インスタンスは自動解釈される
// for all i(0,2,...,N-1) f(xi)≡f(yi) をみたすN-1次多項式を求める(これは一意に定まる)
template<typename Mint>
FPS<Mint> polynomial_interpolation(const vector<Mint> &xs, const vector<Mint> &ys) {
    assert(xs.size() == ys.size());
    vector<FPS<Mint>> mul = subproduct_tree(xs);
    int n = (int)xs.size(), k = (int)mul.size() / 2;
    vector<FPS<Mint>> g(2*k);
    g[1] = mul[1].diff() % mul[1];
    for(int i = 2; i < k+n; i++) g[i] = g[i>>1]%mul[i];
    for(int i = 0; i < n; i++) g[k+i] = {ys[i] / g[k+i][0]};
    for(int i = k; i-- > 1;) g[i] = g[i<<1] * mul[i<<1 | 1] + g[i<<1 | 1] * mul[i<<1];
    return g[1];
}