//calc Π(1+a_i x)
template<typename M>
vector<M> multi_linear_production(const vector<M> &x, int l, int r) {
    if(r-l == 1) return {1, x[l]};

    int m = (l + r) / 2;
    return convolution(multi_linear_production(x, l, m), multi_linear_production(x, m, r));
}
template<typename M>
vector<M> multi_linear_production(const vector<M> &x) {
    return multi_linear_production(x, 0, (int)x.size());
}