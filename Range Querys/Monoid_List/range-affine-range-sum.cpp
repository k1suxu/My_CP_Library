using S = pair<mint, mint>;
S op(S x, S y) {
    return {x.first+y.first, x.second+y.second};
}
S e() {
    return {0, 0};
}
using F = array<mint, 2>;
S mapping(F f, S x) {
    return {f[0]*x.first + f[1]*x.second, x.second};
}
F composition(F latter, F former) {
    return {latter[0]*former[0], latter[0]*former[1] + latter[1]};
}
F id() {
    return {1, 0};
}