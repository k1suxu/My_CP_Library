#include"atcoder/all"
using mint = atcoder::modint998244353;
// using mint = atcoder::modint1000000007;

mint inv(mint x) {
    mint res = 1;
    ll k = mint::mod() - 2;
    mint y = x; 
    while (k) {
        if (k & 1) res = (res * y);
        y = (y * y);
        k /= 2;
    }
    return res;
}
//---------------------------------------------------------------------------------------------------
vector<mint> f = {1}, rf = {1};
void init(int n) {
    f.resize(n, 0);
    rf.resize(n, 0);
    f[0] = 1;
    repi(i, 1, n) f[i] = (f[i - 1] * i);
    repi(i, 0, n) rf[i] = inv(f[i]);
}
mint P(int n, int k) {
    assert(n>=k);
    while(n > f.size()-1) {
        f.push_back(f.back() * f.size());
        rf.push_back(f.back().inv());
    }
    return f[n] * f[n-k];
}
mint C(int n, int k) {
    assert(n>=k);
    while(n > f.size()-1) {
        f.push_back(f.back() * f.size());
        rf.push_back(f.back().inv());
    }
    return f[n]*rf[n-k]*rf[k];
}
mint H(int n, int k) {
    assert(n>=1);
    return C(n+k-1, k);
}

mint Cat(int n) {
    return C(2*n, n)-C(2*n, n-1);
}