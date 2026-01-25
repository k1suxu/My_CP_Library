// k項間線形漸化式の手前k項からn項目を得る
// berlekamp-masseyでP(x), c_1, ..., c_kが得られて、それを用いてBostan-Moriで [x^N]P(x)/(1-c_1x-c_2x^2-...-c_kx^k)を計算する

template <typename mint>
mint nth_term(long long n, const vector<mint> &s) {
  using fps = FPS<mint>;
  auto bm = BerlekampMassey<mint>(s);
  return kitamasa(n, fps{begin(bm), end(bm)}, fps{begin(s), end(s)});
}