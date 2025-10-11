constexpr int B = 5;
int rh_mods[B] = {998244353, 1000000007, 1000000009, 1000000021, 1000000033};
int base[B];
struct Hash {
    long long h1, h2, pw;
};
using S = array<Hash, B>;
S op(S lhs, S rhs) {
    S res;
    for (int i = 0; i < B; i++) {
        res[i].h1 = (lhs[i].h1 * rhs[i].pw + rhs[i].h1) % rh_mods[i];
        res[i].h2 = (lhs[i].h2 + lhs[i].pw * rhs[i].h2) % rh_mods[i];
        res[i].pw = lhs[i].pw * rhs[i].pw % rh_mods[i];
    }
    return res;
}
S op_rev(S lhs, S rhs) {
    return op(rhs, lhs);
}
S e() {
    S res;
    for (int i = 0; i < B; i++) res[i] = {0, 0, 1};
    return res;
}

template<typename T>
S gen(T c) {
    S res;
    for (int i = 0; i < B; i++) res[i].h1 = res[i].h2 = c, res[i].pw = base[i];
    return res;
}
void base_initialize() {
    mt19937_64 rng(time(0));
    for (int i = 0; i < B; i++) {
        base[i] = rng() % rh_mods[i];
    }
}
bool compare(S x, S y) {
    for (int i = 0; i < B; i++) if (x[i].h1 != y[i].h1) return false;
    return true;
}
bool is_palindrome(S x) {
    for (int i = 0; i < B; i++) if (x[i].h1 != x[i].h2) return false;
    return true;
}
/*** base_initializeをわすれずにやること！！！  */
/*** reversed-hashも求める ***/