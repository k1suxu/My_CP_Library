template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> faulhaberOGF(int k) {
    Inner_FPS<Mint> b = bernoulliOGF<Inner_FPS, Mint>(k + 1);
    if (k + 1 > 1) {
        b[1] = -b[1];
    }
    Inner_FPS<Mint> f(k + 2, 0);
    mint ik = mint(k + 1).inv();
    rep(j, k + 1) {
        f[k + 1 - j] = ik * C(k + 1, j) * b[j];
    }
    return f;
}