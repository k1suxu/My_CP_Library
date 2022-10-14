//Calculation
//prime,divisor,factorize
//fast_prime_factorize
//fast_prime_factorize_smart_sieve
class smart_sieve {
  ll L, R, M;
  std::vector<int> small;  // 小さい篩
  std::vector<std::vector<ll>> large;  // 大きい篩
  std::vector<ll> aux;  // aux[i] := large[i] の素因数の積

public:
  smart_sieve(ll L, ll R): L(L), R(R), M(sqrtl(R)+1) {
    small.resize(M);
    std::iota(small.begin(), small.end(), 0);
    large.resize(R-L);
    aux.assign(R-L, 1);

    for (ll i = 2; i*i < R; ++i) {
      if (small[i] < i) continue;
      small[i] = i;
      for (ll j = i*i; j < M; j += i)
        if (small[j] == j) small[j] = i;

      for (ll j = (L+i-1)/i*i; j < R; j += i) {
        ll k = j;
        do {
          // aux[j-L] > M で判定した方がいいかも？
          // j / aux[j-L] < M の方がいい？（割り算したくない）
          if (aux[j-L] * aux[j-L] > R) break;

          large[j-L].push_back(i);
          aux[j-L] *= i;
          k /= i;
        } while (k % i == 0);
      }
    }
  }

  std::vector<ll> factor(ll n) {
    // assert(L <= n && n < R);
    std::vector<ll> res = large[n-L];
    n /= aux[n-L];
    if (n >= M) {
      // この場合，n は素数となることが示せる（はず）
      // n*n >= R だとオーバーフローしそう？
      res.push_back(n);
      return res;
    }
    while (n > 1) {
      res.push_back(small[n]);
      n /= small[n];
    }
    return res;
  }
};