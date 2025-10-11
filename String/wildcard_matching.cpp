// need to include acl_FPS_NTT library

std::vector<bool> wildcard_matching(const std::string &s,
                                    const std::string &t) {
  assert(s.size() >= t.size());
  static constexpr int mod = 998244353;
  using mint = static_modint<mod>;
  int n = 1;
  while (n < s.size())
    n *= 2;
  std::vector<mint> x(n), y(n), z(n);
  std::mt19937_64 rng(std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count());
  std::uniform_int_distribution<int> dist(0, mod - 1);
  for (int i = 0; i < t.size(); i++) {
    if (t[i] != '*') {
      mint r = dist(rng);
      y[-i & (n - 1)] = r;
      z[-i & (n - 1)] = r * static_cast<int>(t[i]);
    }
  }
  internal::butterfly(y);
  internal::butterfly(z);
  for (int i = 0; i < s.size(); i++)
    if (s[i] != '*')
      x[i] = static_cast<int>(s[i]);
  internal::butterfly(x);
  for (int i = 0; i < n; i++)
    x[i] *= y[i];
  for (int i = 0; i < s.size(); i++)
    y[i] = s[i] == '*' ? 0 : 1;
  internal::butterfly(y);
  for (int i = 0; i < n; i++)
    x[i] -= y[i] * z[i];
  internal::butterfly_inv(x);
  std::vector<bool> ret(s.size() - t.size() + 1);
  for (int i = 0; i < ret.size(); i++)
    ret[i] = x[i] == 0;
  return ret;
}

// ワイルドカードはアスタリスクで表す
// ref: 