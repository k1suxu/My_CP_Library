// apply range affineだけのタイプ
struct S {
    mint sum;
    int len;
};
S op(S x, S y) {
    return {x.sum + y.sum, x.len + y.len};
}
S e() {
    return {0, 0};
}
// F[0]x + F[1]
using F = array<mint, 2>;
S mapping(F f, S x) {
    return {x.sum * f[0] + f[1] * x.len, x.len};
}
F composition(F latter, F former) {
    return {latter[0]*former[0], latter[0]*former[1] + latter[1]};
}
F id() {
    return {1, 0};
}
F get_add(mint x) {
    return {1, x};
}
F get_mul(mint x) {
    return {x, 0};
}


// a[i], b[i]を載せるタイプ
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