struct S {
    int l, r;
    bool valid;
};
S op(S x, S y) {
    // check whether x is e() or y is e()
    if(x.l == INF && x.r == -INF) return y;
    if(y.l == INF && y.r == -INF) return x;
    // x.r <= y.l : weakly increasing
    // x.r < y.l  : stringly increasing
    return {x.l, y.r, x.valid && y.valid && x.r <= y.l};
}
S e() {
    return {INF, -INF, false};
}