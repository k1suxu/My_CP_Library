// zero-count monoid
struct S {
    ll len, mi, cnt;
    int min_cnt() {
        if(mi == 0) return len - cnt;
        return len;
    }
};
S op(S a, S b) {
    if(a.mi < b.mi) return {a.len+b.len, a.mi, a.cnt};
    else if(a.mi > b.mi) return {a.len+b.len, b.mi, b.cnt};
    return {a.len+b.len, a.mi, a.cnt+b.cnt};
}
S e() {
    return S{0, INF, 0};
}
using F = int;
S mapping(F f, S x) {
    return S{x.len, min(INF, x.mi+f), x.cnt};
}
F composition(F a, F b) {
    return a + b;
}
F id() { return 0; }