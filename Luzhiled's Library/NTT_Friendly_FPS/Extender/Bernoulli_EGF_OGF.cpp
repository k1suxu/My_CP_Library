// m-1次まで
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> bernoulliEGF(int m) {
    Inner_FPS<Mint> f(m + 1);
    for (int i = 0; i <= m; i++) f[i] = fact_inv[i + 1];
    return f.inv(m);
}
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> bernoulliOGF(int m) {
    Inner_FPS<Mint> egf = bernoulliEGF<Inner_FPS, Mint>(m);
    for (int i = 0; i < m; i++) egf[i] *= fact[i];
    return egf;
}