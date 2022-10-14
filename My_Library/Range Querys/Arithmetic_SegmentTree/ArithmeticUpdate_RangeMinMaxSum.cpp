//refer : https://null-mn.hatenablog.com/entry/2021/08/22/064325

#include"atcoder/all"

const ll LINF = ll(4e18);
const int INF = int(1e9) + 10;

struct S {
    ll sum;
    int l, r;
    S() = default;
    S(ll sum, int l, int r) : sum(sum), l(l), r(r) {}
};
struct F {
    ll a, b;
    F() = default;
    F(ll a, ll b) : a(a), b(b) {}
};
S op(S s, S t) { return {s.sum + t.sum, min(s.l, t.l), max(s.r, t.r)}; }
S e() { return {0, INF, -INF}; }
S mapping(F f, S s) {
    return {s.sum + (f.a * (s.l + s.r - 1) + f.b * 2) * (s.r - s.l) / 2, s.l, s.r};
}
F composition(F f, F g) { return {f.a + g.a, f.b + g.b}; }
F id() { return {0, 0}; }

using lazy_segtype = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;

struct lazy_segtree_arithmetic : lazy_segtype {
    using lazy_segtype::lazy_segtype;
    lazy_segtree_arithmetic(int n) {
        vector<S> sv(n);
        for (int i = 0; i < n; i++) sv[i] = {0, i, i + 1};
        (*this) = lazy_segtree_arithmetic(sv);
    }
    lazy_segtree_arithmetic(vector<ll> v) {
        vector<S> sv(v.size());
        for (int i = 0; i < v.size(); i++) sv[i] = {v[i], i, i + 1};
        (*this) = lazy_segtree_arithmetic(sv);
    }
    // f = (a, b)
    // [l, r) ‚É b, a+b, 2a+b, ... ‚ð‰ÁŽZ
    void apply(int l, int r, F f) {
        lazy_segtype::apply(l, r, {f.a, f.b - f.a * get(l).l});
    }
    void apply(int p, F f) { apply(p, {f.a, f.b - f.a * get(p).l}); }
};