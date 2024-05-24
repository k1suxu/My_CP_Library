#include <cassert>
#include <numeric>
#include <type_traits>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace internal {

#ifndef _MSC_VER
template <class T>
using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value ||
                                  std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                  std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;

template <class T>
using is_integral = typename std::conditional<std::is_integral<T>::value ||
                                                  is_signed_int128<T>::value ||
                                                  is_unsigned_int128<T>::value,
                                              std::true_type,
                                              std::false_type>::type;

template <class T>
using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                 std::is_signed<T>::value) ||
                                                    is_signed_int128<T>::value,
                                                std::true_type,
                                                std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<(is_integral<T>::value &&
                               std::is_unsigned<T>::value) ||
                                  is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<
    is_signed_int128<T>::value,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;

#else

template <class T> using is_integral = typename std::is_integral<T>;

template <class T>
using is_signed_int =
    typename std::conditional<is_integral<T>::value && std::is_signed<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<is_integral<T>::value &&
                                  std::is_unsigned<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<is_signed_int<T>::value,
                                              std::make_unsigned<T>,
                                              std::common_type<T>>::type;

#endif

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

namespace internal {

// @param m `1 <= m`
// @return x mod m
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Fast modular multiplication by barrett reduction
// Reference: https://en.wikipedia.org/wiki/Barrett_reduction
// NOTE: reconsider after Ice Lake
struct barrett {
    unsigned int _m;
    unsigned long long im;

    // @param m `1 <= m < 2^31`
    explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}

    // @return m
    unsigned int umod() const { return _m; }

    // @param a `0 <= a < m`
    // @param b `0 <= b < m`
    // @return `a * b % m`
    unsigned int mul(unsigned int a, unsigned int b) const {
        // [1] m = 1
        // a = b = im = 0, so okay

        // [2] m >= 2
        // im = ceil(2^64 / m)
        // -> im * m = 2^64 + r (0 <= r < m)
        // let z = a*b = c*m + d (0 <= c, d < m)
        // a*b * im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 + c*r + d*im
        // c*r + d*im < m * m + m * im < m * m + 2^64 + m <= 2^64 + m * (m + 1) < 2^64 * 2
        // ((ab * im) >> 64) == c or c + 1
        unsigned long long z = a;
        z *= b;
#ifdef _MSC_VER
        unsigned long long x;
        _umul128(z, im, &x);
#else
        unsigned long long x =
            (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
#endif
        unsigned int v = (unsigned int)(z - x * _m);
        if (_m <= v) v += _m;
        return v;
    }
};

// @param n `0 <= n`
// @param m `1 <= m`
// @return `(x ** n) % m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

// Reference:
// M. Forisek and J. Jancina,
// Fast Primality Testing for Integers That Fit into a Machine Word
// @param n `0 <= n`
constexpr bool is_prime_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
template <int n> constexpr bool is_prime = is_prime_constexpr(n);

// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};

    // Contracts:
    // [1] s - m0 * a = 0 (mod b)
    // [2] t - m1 * a = 0 (mod b)
    // [3] s * |m1| + t * |m0| <= b
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;

    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;  // |m1 * u| <= |m1| * s <= b

        // [3]:
        // (s - t * u) * |m1| + t * |m0 - m1 * u|
        // <= s * |m1| - t * u * |m1| + t * (|m0| + |m1| * u)
        // = s * |m1| + t * |m0| <= b

        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    // by [3]: |m0| <= b/g
    // by g != b: |m0| < b/g
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}

// Compile time primitive root
// @param m must be prime
// @return primitive root (and minimum in now)
constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) {
        divs[cnt++] = x;
    }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_constexpr(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
template <int m> constexpr int primitive_root = primitive_root_constexpr(m);

// @param n `n < 2^32`
// @param m `1 <= m < 2^32`
// @return sum_{i=0}^{n-1} floor((ai + b) / m) (mod 2^64)
unsigned long long floor_sum_unsigned(unsigned long long n,
                                      unsigned long long m,
                                      unsigned long long a,
                                      unsigned long long b) {
    unsigned long long ans = 0;
    while (true) {
        if (a >= m) {
            ans += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }

        unsigned long long y_max = a * n + b;
        if (y_max < m) break;
        // y_max < m * (n + 1)
        // floor(y_max / m) <= n
        n = (unsigned long long)(y_max / m);
        b = (unsigned long long)(y_max % m);
        std::swap(m, a);
    }
    return ans;
}

}  // namespace internal

namespace internal {

struct modint_base {};
struct static_modint_base : modint_base {};

template <class T> using is_modint = std::is_base_of<modint_base, T>;
template <class T> using is_modint_t = std::enable_if_t<is_modint<T>::value>;

}  // namespace internal

template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
struct static_modint : internal::static_modint_base {
    using mint = static_modint;

  public:
    static constexpr int mod() { return m; }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    static_modint() : _v(0) {}
    template <class T, internal::is_signed_int_t<T>* = nullptr>
    static_modint(T v) {
        long long x = (long long)(v % (long long)(umod()));
        if (x < 0) x += umod();
        _v = (unsigned int)(x);
    }
    template <class T, internal::is_unsigned_int_t<T>* = nullptr>
    static_modint(T v) {
        _v = (unsigned int)(v % umod());
    }

    unsigned int val() const { return _v; }

    mint& operator++() {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator--() {
        if (_v == 0) _v = umod();
        _v--;
        return *this;
    }

    mint& operator+=(const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        _v -= rhs._v;
        if (_v >= umod()) _v += umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        unsigned long long z = _v;
        z *= rhs._v;
        _v = (unsigned int)(z % umod());
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }

    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        if (prime) {
            assert(_v);
            return pow(umod() - 2);
        } else {
            auto eg = internal::inv_gcd(_v, m);
            assert(eg.first == 1);
            return eg.second;
        }
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static constexpr unsigned int umod() { return m; }
    static constexpr bool prime = internal::is_prime<m>;
};

template <int id> struct dynamic_modint : internal::modint_base {
    using mint = dynamic_modint;

  public:
    static int mod() { return (int)(bt.umod()); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = internal::barrett(m);
    }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    dynamic_modint() : _v(0) {}
    template <class T, internal::is_signed_int_t<T>* = nullptr>
    dynamic_modint(T v) {
        long long x = (long long)(v % (long long)(mod()));
        if (x < 0) x += mod();
        _v = (unsigned int)(x);
    }
    template <class T, internal::is_unsigned_int_t<T>* = nullptr>
    dynamic_modint(T v) {
        _v = (unsigned int)(v % mod());
    }

    unsigned int val() const { return _v; }

    mint& operator++() {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator--() {
        if (_v == 0) _v = umod();
        _v--;
        return *this;
    }

    mint& operator+=(const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        _v += mod() - rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        _v = bt.mul(_v, rhs._v);
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }

    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        auto eg = internal::inv_gcd(_v, mod());
        assert(eg.first == 1);
        return eg.second;
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static internal::barrett bt;
    static unsigned int umod() { return bt.umod(); }
};
template <int id> internal::barrett dynamic_modint<id>::bt(998244353);

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;

namespace internal {

template <class T>
using is_static_modint = std::is_base_of<internal::static_modint_base, T>;

template <class T>
using is_static_modint_t = std::enable_if_t<is_static_modint<T>::value>;

template <class> struct is_dynamic_modint : public std::false_type {};
template <int id>
struct is_dynamic_modint<dynamic_modint<id>> : public std::true_type {};

template <class T>
using is_dynamic_modint_t = std::enable_if_t<is_dynamic_modint<T>::value>;

}  // namespace internal

namespace internal {

// @param n `0 <= n`
// @return minimum non-negative `x` s.t. `n <= 2**x`
int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
constexpr int bsf_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

}  // namespace internal

namespace internal {

template <class mint,
          int g = internal::primitive_root<mint::mod()>,
          internal::is_static_modint_t<mint>* = nullptr>
struct fft_info {
    static constexpr int rank2 = bsf_constexpr(mint::mod() - 1);
    std::array<mint, rank2 + 1> root;   // root[i]^(2^i) == 1
    std::array<mint, rank2 + 1> iroot;  // root[i] * iroot[i] == 1

    std::array<mint, std::max(0LL, rank2 - 2 + 1)> rate2;
    std::array<mint, std::max(0LL, rank2 - 2 + 1)> irate2;

    std::array<mint, std::max(0LL, rank2 - 3 + 1)> rate3;
    std::array<mint, std::max(0LL, rank2 - 3 + 1)> irate3;

    fft_info() {
        root[rank2] = mint(g).pow((mint::mod() - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        for (int i = rank2 - 1; i >= 0; i--) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }

        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 2; i++) {
                rate2[i] = root[i + 2] * prod;
                irate2[i] = iroot[i + 2] * iprod;
                prod *= iroot[i + 2];
                iprod *= root[i + 2];
            }
        }
        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 3; i++) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }
    }
};

template <class mint, internal::is_static_modint_t<mint>* = nullptr>
void butterfly(std::vector<mint>& a) {
    int n = (int)(a.size());
    int h = internal::ceil_pow2(n);

    static const fft_info<mint> info;

    int len = 0;  // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
    while (len < h) {
        if (h - len == 1) {
            int p = 1 << (h - len - 1);
            mint rot = 1;
            for (int s = 0; s < (1 << len); s++) {
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * rot;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                if (s + 1 != (1 << len))
                    rot *= info.rate2[bsf(~(unsigned int)(s))];
            }
            len++;
        } else {
            // 4-base
            int p = 1 << (h - len - 2);
            mint rot = 1, imag = info.root[2];
            for (int s = 0; s < (1 << len); s++) {
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto mod2 = 1ULL * mint::mod() * mint::mod();
                    auto a0 = 1ULL * a[i + offset].val();
                    auto a1 = 1ULL * a[i + offset + p].val() * rot.val();
                    auto a2 = 1ULL * a[i + offset + 2 * p].val() * rot2.val();
                    auto a3 = 1ULL * a[i + offset + 3 * p].val() * rot3.val();
                    auto a1na3imag =
                        1ULL * mint(a1 + mod2 - a3).val() * imag.val();
                    auto na2 = mod2 - a2;
                    a[i + offset] = a0 + a2 + a1 + a3;
                    a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                    a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                    a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                }
                if (s + 1 != (1 << len))
                    rot *= info.rate3[bsf(~(unsigned int)(s))];
            }
            len += 2;
        }
    }
}

template <class mint, internal::is_static_modint_t<mint>* = nullptr>
void butterfly_inv(std::vector<mint>& a) {
    int n = (int)(a.size());
    int h = internal::ceil_pow2(n);

    static const fft_info<mint> info;

    int len = h;  // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
    while (len) {
        if (len == 1) {
            int p = 1 << (h - len);
            mint irot = 1;
            for (int s = 0; s < (1 << (len - 1)); s++) {
                int offset = s << (h - len + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] =
                        (unsigned long long)(mint::mod() + l.val() - r.val()) *
                        irot.val();
                    ;
                }
                if (s + 1 != (1 << (len - 1)))
                    irot *= info.irate2[bsf(~(unsigned int)(s))];
            }
            len--;
        } else {
            // 4-base
            int p = 1 << (h - len);
            mint irot = 1, iimag = info.iroot[2];
            for (int s = 0; s < (1 << (len - 2)); s++) {
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h - len + 2);
                for (int i = 0; i < p; i++) {
                    auto a0 = 1ULL * a[i + offset + 0 * p].val();
                    auto a1 = 1ULL * a[i + offset + 1 * p].val();
                    auto a2 = 1ULL * a[i + offset + 2 * p].val();
                    auto a3 = 1ULL * a[i + offset + 3 * p].val();

                    auto a2na3iimag =
                        1ULL *
                        mint((mint::mod() + a2 - a3) * iimag.val()).val();

                    a[i + offset] = a0 + a1 + a2 + a3;
                    a[i + offset + 1 * p] =
                        (a0 + (mint::mod() - a1) + a2na3iimag) * irot.val();
                    a[i + offset + 2 * p] =
                        (a0 + a1 + (mint::mod() - a2) + (mint::mod() - a3)) *
                        irot2.val();
                    a[i + offset + 3 * p] =
                        (a0 + (mint::mod() - a1) + (mint::mod() - a2na3iimag)) *
                        irot3.val();
                }
                if (s + 1 != (1 << (len - 2)))
                    irot *= info.irate3[bsf(~(unsigned int)(s))];
            }
            len -= 2;
        }
    }
}

template <class mint, internal::is_static_modint_t<mint>* = nullptr>
std::vector<mint> convolution_naive(const std::vector<mint>& a,
                                    const std::vector<mint>& b) {
    int n = (int)(a.size()), m = (int)(b.size());
    std::vector<mint> ans(n + m - 1);
    if (n < m) {
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                ans[i + j] += a[i] * b[j];
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans[i + j] += a[i] * b[j];
            }
        }
    }
    return ans;
}

template <class mint, internal::is_static_modint_t<mint>* = nullptr>
std::vector<mint> convolution_fft(std::vector<mint> a, std::vector<mint> b) {
    int n = (int)(a.size()), m = (int)(b.size());
    int z = 1 << internal::ceil_pow2(n + m - 1);
    a.resize(z);
    internal::butterfly(a);
    b.resize(z);
    internal::butterfly(b);
    for (int i = 0; i < z; i++) {
        a[i] *= b[i];
    }
    internal::butterfly_inv(a);
    a.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) a[i] *= iz;
    return a;
}

}  // namespace internal

template <class mint, internal::is_static_modint_t<mint>* = nullptr>
std::vector<mint> convolution(std::vector<mint>&& a, std::vector<mint>&& b) {
    int n = (int)(a.size()), m = (int)(b.size());
    if (!n || !m) return {};
    if (std::min(n, m) <= 60) return convolution_naive(a, b);
    return internal::convolution_fft(a, b);
}

template <class mint, internal::is_static_modint_t<mint>* = nullptr>
std::vector<mint> convolution(const std::vector<mint>& a,
                              const std::vector<mint>& b) {
    int n = (int)(a.size()), m = (int)(b.size());
    if (!n || !m) return {};
    if (std::min(n, m) <= 60) return convolution_naive(a, b);
    return internal::convolution_fft(a, b);
}

template <unsigned int mod = 998244353,
          class T,
          std::enable_if_t<internal::is_integral<T>::value>* = nullptr>
std::vector<T> convolution(const std::vector<T>& a, const std::vector<T>& b) {
    int n = (int)(a.size()), m = (int)(b.size());
    if (!n || !m) return {};

    using mint = static_modint<mod>;
    std::vector<mint> a2(n), b2(m);
    for (int i = 0; i < n; i++) {
        a2[i] = mint(a[i]);
    }
    for (int i = 0; i < m; i++) {
        b2[i] = mint(b[i]);
    }
    auto c2 = convolution(move(a2), move(b2));
    std::vector<T> c(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        c[i] = c2[i].val();
    }
    return c;
}

std::vector<long long> convolution_ll(const std::vector<long long>& a,
                                      const std::vector<long long>& b) {
    int n = (int)(a.size()), m = (int)(b.size());
    if (!n || !m) return {};

    static constexpr unsigned long long MOD1 = 754974721;  // 2^24
    static constexpr unsigned long long MOD2 = 167772161;  // 2^25
    static constexpr unsigned long long MOD3 = 469762049;  // 2^26
    static constexpr unsigned long long M2M3 = MOD2 * MOD3;
    static constexpr unsigned long long M1M3 = MOD1 * MOD3;
    static constexpr unsigned long long M1M2 = MOD1 * MOD2;
    static constexpr unsigned long long M1M2M3 = MOD1 * MOD2 * MOD3;

    static constexpr unsigned long long i1 =
        internal::inv_gcd(MOD2 * MOD3, MOD1).second;
    static constexpr unsigned long long i2 =
        internal::inv_gcd(MOD1 * MOD3, MOD2).second;
    static constexpr unsigned long long i3 =
        internal::inv_gcd(MOD1 * MOD2, MOD3).second;

    auto c1 = convolution<MOD1>(a, b);
    auto c2 = convolution<MOD2>(a, b);
    auto c3 = convolution<MOD3>(a, b);

    std::vector<long long> c(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        unsigned long long x = 0;
        x += (c1[i] * i1) % MOD1 * M2M3;
        x += (c2[i] * i2) % MOD2 * M1M3;
        x += (c3[i] * i3) % MOD3 * M1M2;
        // B = 2^63, -B <= x, r(real value) < B
        // (x, x - M, x - 2M, or x - 3M) = r (mod 2B)
        // r = c1[i] (mod MOD1)
        // focus on MOD1
        // r = x, x - M', x - 2M', x - 3M' (M' = M % 2^64) (mod 2B)
        // r = x,
        //     x - M' + (0 or 2B),
        //     x - 2M' + (0, 2B or 4B),
        //     x - 3M' + (0, 2B, 4B or 6B) (without mod!)
        // (r - x) = 0, (0)
        //           - M' + (0 or 2B), (1)
        //           -2M' + (0 or 2B or 4B), (2)
        //           -3M' + (0 or 2B or 4B or 6B) (3) (mod MOD1)
        // we checked that
        //   ((1) mod MOD1) mod 5 = 2
        //   ((2) mod MOD1) mod 5 = 3
        //   ((3) mod MOD1) mod 5 = 4
        long long diff =
            c1[i] - internal::safe_mod((long long)(x), (long long)(MOD1));
        if (diff < 0) diff += MOD1;
        static constexpr unsigned long long offset[5] = {
            0, 0, M1M2M3, 2 * M1M2M3, 3 * M1M2M3};
        x -= offset[diff % 5];
        c[i] = x;
    }

    return c;
}

//need to include mod_int structure

//ref: https://ei1333.github.io/library/math/fft/number-theoretic-transform-friendly-mod-int.hpp
template< typename Mint >
struct NumberTheoreticTransformFriendlyModInt {

  static vector< Mint > roots, iroots, rate3, irate3;
  static int max_base;

  NumberTheoreticTransformFriendlyModInt() = default;

  static void init() {
    if(roots.empty()) {
      const unsigned mod = Mint::mod();
      assert(mod >= 3 && mod % 2 == 1);
      auto tmp = mod - 1;
      max_base = 0;
      while(tmp % 2 == 0) tmp >>= 1, max_base++;
      Mint root = 2;
      while(root.pow((mod - 1) >> 1) == 1) {
        root += 1;
      }
      assert(root.pow(mod - 1) == 1);

      roots.resize(max_base + 1);
      iroots.resize(max_base + 1);
      rate3.resize(max_base + 1);
      irate3.resize(max_base + 1);

      roots[max_base] = root.pow((mod - 1) >> max_base);
      iroots[max_base] = Mint(1) / roots[max_base];
      for(int i = max_base - 1; i >= 0; i--) {
        roots[i] = roots[i + 1] * roots[i + 1];
        iroots[i] = iroots[i + 1] * iroots[i + 1];
      }
      {
        Mint prod = 1, iprod = 1;
        for(int i = 0; i <= max_base - 3; i++) {
          rate3[i] = roots[i + 3] * prod;
          irate3[i] = iroots[i + 3] * iprod;
          prod *= iroots[i + 3];
          iprod *= roots[i + 3];
        }
      }
    }
  }

  static void ntt(vector< Mint > &a) {
    init();
    const int n = (int) a.size();
    assert((n & (n - 1)) == 0);
    int h = __builtin_ctz(n);
    assert(h <= max_base);
    int len = 0;
    Mint imag = roots[2];
    if(h & 1) {
      int p = 1 << (h - 1);
      Mint rot = 1;
      for(int i = 0; i < p; i++) {
        auto r = a[i + p];
        a[i + p] = a[i] - r;
        a[i] += r;
      }
      len++;
    }
    for(; len + 1 < h; len += 2) {
      int p = 1 << (h - len - 2);
      { // s = 0
        for(int i = 0; i < p; i++) {
          auto a0 = a[i];
          auto a1 = a[i + p];
          auto a2 = a[i + 2 * p];
          auto a3 = a[i + 3 * p];
          auto a1na3imag = (a1 - a3) * imag;
          auto a0a2 = a0 + a2;
          auto a1a3 = a1 + a3;
          auto a0na2 = a0 - a2;
          a[i] = a0a2 + a1a3;
          a[i + 1 * p] = a0a2 - a1a3;
          a[i + 2 * p] = a0na2 + a1na3imag;
          a[i + 3 * p] = a0na2 - a1na3imag;
        }
      }
      Mint rot = rate3[0];
      for(int s = 1; s < (1 << len); s++) {
        int offset = s << (h - len);
        Mint rot2 = rot * rot;
        Mint rot3 = rot2 * rot;
        for(int i = 0; i < p; i++) {
          auto a0 = a[i + offset];
          auto a1 = a[i + offset + p] * rot;
          auto a2 = a[i + offset + 2 * p] * rot2;
          auto a3 = a[i + offset + 3 * p] * rot3;
          auto a1na3imag = (a1 - a3) * imag;
          auto a0a2 = a0 + a2;
          auto a1a3 = a1 + a3;
          auto a0na2 = a0 - a2;
          a[i + offset] = a0a2 + a1a3;
          a[i + offset + 1 * p] = a0a2 - a1a3;
          a[i + offset + 2 * p] = a0na2 + a1na3imag;
          a[i + offset + 3 * p] = a0na2 - a1na3imag;
        }
        rot *= rate3[__builtin_ctz(~s)];
      }
    }
  }

  static void intt(vector< Mint > &a, bool f = true) {
    init();
    const int n = (int) a.size();
    assert((n & (n - 1)) == 0);
    int h = __builtin_ctz(n);
    assert(h <= max_base);
    int len = h;
    Mint iimag = iroots[2];
    for(; len > 1; len -= 2) {
      int p = 1 << (h - len);
      { // s = 0
        for(int i = 0; i < p; i++) {
          auto a0 = a[i];
          auto a1 = a[i + 1 * p];
          auto a2 = a[i + 2 * p];
          auto a3 = a[i + 3 * p];
          auto a2na3iimag = (a2 - a3) * iimag;
          auto a0na1 = a0 - a1;
          auto a0a1 = a0 + a1;
          auto a2a3 = a2 + a3;
          a[i] = a0a1 + a2a3;
          a[i + 1 * p] = (a0na1 + a2na3iimag);
          a[i + 2 * p] = (a0a1 - a2a3);
          a[i + 3 * p] = (a0na1 - a2na3iimag);
        }
      }
      Mint irot = irate3[0];
      for(int s = 1; s < (1 << (len - 2)); s++) {
        int offset = s << (h - len + 2);
        Mint irot2 = irot * irot;
        Mint irot3 = irot2 * irot;
        for(int i = 0; i < p; i++) {
          auto a0 = a[i + offset];
          auto a1 = a[i + offset + 1 * p];
          auto a2 = a[i + offset + 2 * p];
          auto a3 = a[i + offset + 3 * p];
          auto a2na3iimag = (a2 - a3) * iimag;
          auto a0na1 = a0 - a1;
          auto a0a1 = a0 + a1;
          auto a2a3 = a2 + a3;
          a[i + offset] = a0a1 + a2a3;
          a[i + offset + 1 * p] = (a0na1 + a2na3iimag) * irot;
          a[i + offset + 2 * p] = (a0a1 - a2a3) * irot2;
          a[i + offset + 3 * p] = (a0na1 - a2na3iimag) * irot3;
        }
        irot *= irate3[__builtin_ctz(~s)];
      }
    }
    if(len >= 1) {
      int p = 1 << (h - 1);
      for(int i = 0; i < p; i++) {
        auto ajp = a[i] - a[i + p];
        a[i] += a[i + p];
        a[i + p] = ajp;
      }
    }
    if(f) {
      Mint inv_sz = Mint(1) / n;
      for(int i = 0; i < n; i++) a[i] *= inv_sz;
    }
  }

  static vector< Mint > multiply(vector< Mint > a, vector< Mint > b) {
    int need = a.size() + b.size() - 1;
    int nbase = 1;
    while((1 << nbase) < need) nbase++;
    int sz = 1 << nbase;
    a.resize(sz, 0);
    b.resize(sz, 0);
    ntt(a);
    ntt(b);
    Mint inv_sz = Mint(1) / sz;
    for(int i = 0; i < sz; i++) a[i] *= b[i] * inv_sz;
    intt(a, false);
    a.resize(need);
    return a;
  }
};

template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::roots = vector< Mint >();
template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::iroots = vector< Mint >();
template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::rate3 = vector< Mint >();
template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::irate3 = vector< Mint >();
template< typename Mint >
int NumberTheoreticTransformFriendlyModInt< Mint >::max_base = 0;

//ref: https://ei1333.github.io/library/math/fps/formal-power-series-friendly-ntt.hpp
template< typename T >
struct FormalPowerSeriesFriendlyNTT : vector< T > {
    using vector< T >::vector;
    using P = FormalPowerSeriesFriendlyNTT;
    using NTT = NumberTheoreticTransformFriendlyModInt< T >;

    // prefix
    P pre(int deg) const {
        return P(begin(*this), begin(*this) + min((int) this->size(), deg));
    }

    P rev(int deg = -1) const {
        P ret(*this);
        if(deg != -1) ret.resize(deg, T(0));
        reverse(begin(ret), end(ret));
        return ret;
    }

    void shrink() {
        while(this->size() && this->back() == T(0)) this->pop_back();
    }

    P operator+(const P &r) const { return P(*this) += r; }

    P operator+(const T &v) const { return P(*this) += v; }

    P operator-(const P &r) const { return P(*this) -= r; }

    P operator-(const T &v) const { return P(*this) -= v; }

    P operator*(const P &r) const { return P(*this) *= r; }

    P operator*(const T &v) const { return P(*this) *= v; }

    P operator/(const P &r) const { return P(*this) /= r; }

    P operator%(const P &r) const { return P(*this) %= r; }

    P &operator+=(const P &r) {
        if(r.size() > this->size()) this->resize(r.size());
        for(int i = 0; i < (int) r.size(); i++) (*this)[i] += r[i];
        return *this;
    }

    P &operator-=(const P &r) {
        if(r.size() > this->size()) this->resize(r.size());
        for(int i = 0; i < (int) r.size(); i++) (*this)[i] -= r[i];
        return *this;
    }

    // https://judge.yosupo.jp/problem/convolution_mod
    P &operator*=(const P &r) {
        if(this->empty() || r.empty()) {
            this->clear();
            return *this;
        }
        auto ret = NTT::multiply(*this, r);
        return *this = {begin(ret), end(ret)};
    }

    P &operator/=(const P &r) {
        if(this->size() < r.size()) {
            this->clear();
            return *this;
        }
        int n = this->size() - r.size() + 1;
        return *this = (rev().pre(n) * r.rev().inv(n)).pre(n).rev(n);
    }

    P &operator%=(const P &r) {
        *this -= *this / r * r;
        shrink();
        return *this;
    }

    // https://judge.yosupo.jp/problem/division_of_polynomials
    pair< P, P > div_mod(const P &r) {
        P q = *this / r;
        P x = *this - q * r;
        x.shrink();
        return make_pair(q, x);
    }

    P operator-() const {
        P ret(this->size());
        for(int i = 0; i < (int) this->size(); i++) ret[i] = -(*this)[i];
        return ret;
    }

    P &operator+=(const T &r) {
        if(this->empty()) this->resize(1);
        (*this)[0] += r;
        return *this;
    }

    P &operator-=(const T &r) {
        if(this->empty()) this->resize(1);
        (*this)[0] -= r;
        return *this;
    }

    P &operator*=(const T &v) {
        for(int i = 0; i < (int) this->size(); i++) (*this)[i] *= v;
        return *this;
    }

    P dot(P r) const {
        P ret(min(this->size(), r.size()));
        for(int i = 0; i < (int) ret.size(); i++) ret[i] = (*this)[i] * r[i];
        return ret;
    }

    P operator>>(int sz) const {
        if((int) this->size() <= sz) return {};
        P ret(*this);
        ret.erase(ret.begin(), ret.begin() + sz);
        return ret;
    }

    P operator<<(int sz) const {
        P ret(*this);
        ret.insert(ret.begin(), sz, T(0));
        return ret;
    }

    T operator()(T x) const {
        T r = 0, w = 1;
        for(auto &v : *this) {
            r += w * v;
            w *= x;
        }
        return r;
    }

    // differential (微分)
    P diff() const {
        const int n = (int) this->size();
        P ret(max((int)0, n-1));
        for(int i = 1; i < n; i++) ret[i - 1] = (*this)[i] * T(i);
        return ret;
    }

    P integral() const {
        const int n = (int) this->size();
        P ret(n + 1);
        ret[0] = T(0);
        for(int i = 0; i < n; i++) ret[i + 1] = (*this)[i] / T(i + 1);
        return ret;
    }

    // https://judge.yosupo.jp/problem/inv_of_formal_power_series
    // F(0) must not be 0
    P inv(int deg = -1) const {
        assert(((*this)[0]) != T(0));
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        P res(deg);
        res[0] = {T(1) / (*this)[0]};
        for(int d = 1; d < deg; d <<= 1) {
            P f(2 * d), g(2 * d);
            for(int j = 0; j < min(n, 2 * d); j++) f[j] = (*this)[j];
            for(int j = 0; j < d; j++) g[j] = res[j];
            NTT::ntt(f);
            NTT::ntt(g);
            f = f.dot(g);
            NTT::intt(f);
            for(int j = 0; j < d; j++) f[j] = 0;
            NTT::ntt(f);
            for(int j = 0; j < 2 * d; j++) f[j] *= g[j];
            NTT::intt(f);
            for(int j = d; j < min(2 * d, deg); j++) res[j] = -f[j];
        }
        return res;
    }

    // https://judge.yosupo.jp/problem/log_of_formal_power_series
    // F(0) must be 1
    P log(int deg = -1) const {
        assert((*this)[0] == T(1));
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        return (this->diff() * this->inv(deg)).pre(deg - 1).integral();
    }

    // https://judge.yosupo.jp/problem/sqrt_of_formal_power_series
    P sqrt(int deg = -1) const {
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        if((*this)[0] == T(0)) {
            for(int i = 1; i < n; i++) {
                if((*this)[i] != T(0)) {
                    if(i & 1) return {};
                    if(deg - i / 2 <= 0) break;
                    auto ret = (*this >> i).sqrt(deg - i / 2);
                    if(ret.empty()) return {};
                    ret = ret << (i / 2);
                    if((int) ret.size() < deg) ret.resize(deg, T(0));
                    return ret;
                }
            }
            return P(deg, 0);
        }
        auto sqr = T((*this)[0].get_sqrt());
        if(sqr * sqr != (*this)[0]) return {};
        P ret{sqr};
        T inv2 = T(1) / T(2);
        for(int i = 1; i < deg; i <<= 1) {
            ret = (ret + pre(i << 1) * ret.inv(i << 1)) * inv2;
        }
        return ret.pre(deg);
    }

    // https://judge.yosupo.jp/problem/exp_of_formal_power_series
    // F(0) must be 0
    P exp(int deg = -1) const {
        if(deg == -1) deg = this->size();
        assert((*this)[0] == T(0));

        P inv;
        inv.reserve(deg + 1);
        inv.push_back(T(0));
        inv.push_back(T(1));

        auto inplace_integral = [&](P &F) -> void {
            const int n = (int) F.size();
            auto mod = T::mod();
            while((int) inv.size() <= n) {
                int i = inv.size();
                inv.push_back((-inv[mod % i]) * (mod / i));
            }
            F.insert(begin(F), T(0));
            for(int i = 1; i <= n; i++) F[i] *= inv[i];
        };

        auto inplace_diff = [](P &F) -> void {
            if(F.empty()) return;
            F.erase(begin(F));
            T coeff = 1, one = 1;
            for(int i = 0; i < (int) F.size(); i++) {
                F[i] *= coeff;
                coeff += one;
            }
        };

        P b{1, 1 < (int) this->size() ? (*this)[1] : 0}, c{1}, z1, z2{1, 1};
        for(int m = 2; m < deg; m *= 2) {
            auto y = b;
            y.resize(2 * m);
            NTT::ntt(y);
            z1 = z2;
            P z(m);
            for(int i = 0; i < m; ++i) z[i] = y[i] * z1[i];
            NTT::intt(z);
            fill(begin(z), begin(z) + m / 2, T(0));
            NTT::ntt(z);
            for(int i = 0; i < m; ++i) z[i] *= -z1[i];
            NTT::intt(z);
            c.insert(end(c), begin(z) + m / 2, end(z));
            z2 = c;
            z2.resize(2 * m);
            NTT::ntt(z2);
            P x(begin(*this), begin(*this) + min< int >(this->size(), m));
            inplace_diff(x);
            x.push_back(T(0));
            NTT::ntt(x);
            for(int i = 0; i < m; ++i) x[i] *= y[i];
            NTT::intt(x);
            x -= b.diff();
            x.resize(2 * m);
            for(int i = 0; i < m - 1; ++i) x[m + i] = x[i], x[i] = T(0);
            NTT::ntt(x);
            for(int i = 0; i < 2 * m; ++i) x[i] *= z2[i];
            NTT::intt(x);
            x.pop_back();
            inplace_integral(x);
            for(int i = m; i < min< int >(this->size(), 2 * m); ++i) x[i] += (*this)[i];
            fill(begin(x), begin(x) + m, T(0));
            NTT::ntt(x);
            for(int i = 0; i < 2 * m; ++i) x[i] *= y[i];
            NTT::intt(x);
            b.insert(end(b), begin(x) + m, end(x));
        }
        return P{begin(b), begin(b) + deg};
    }

    // https://judge.yosupo.jp/problem/pow_of_formal_power_series
    P pow(int64_t k, int deg = -1) const {
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        if(k == 0) {
            P ret(deg, T(0));
            ret[0] = T(1);
            return ret;
        }
        for(int i = 0; i < n; i++) {
            if(i * k > deg) return P(deg, T(0));
            if((*this)[i] != T(0)) {
                T rev = T(1) / (*this)[i];
                P ret = (((*this * rev) >> i).log(deg) * k).exp(deg) * ((*this)[i].pow(k));
                ret = (ret << (i * k)).pre(deg);
                if((int) ret.size() < deg) ret.resize(deg, T(0));
                return ret;
            }
        }
        return *this;
    }

    P mod_pow(int64_t k, P g) const {
        P modinv = g.rev().inv();
        auto get_div = [&](P base) {
            if(base.size() < g.size()) {
                base.clear();
                return base;
            }
            int n = base.size() - g.size() + 1;
            return (base.rev().pre(n) * modinv.pre(n)).pre(n).rev(n);
        };
        P x(*this), ret{1};
        while(k > 0) {
            if(k & 1) {
                ret *= x;
                ret -= get_div(ret) * g;
                ret.shrink();
            }
            x *= x;
            x -= get_div(x) * g;
            x.shrink();
            k >>= 1;
        }
        return ret;
    }

    // https://judge.yosupo.jp/problem/polynomial_taylor_shift
    P taylor_shift(T c) const {
        int n = (int) this->size();
        vector< T > fact(n), rfact(n);
        fact[0] = rfact[0] = T(1);
        for(int i = 1; i < n; i++) fact[i] = fact[i - 1] * T(i);
        rfact[n - 1] = T(1) / fact[n - 1];
        for(int i = n - 1; i > 1; i--) rfact[i - 1] = rfact[i] * T(i);
        P p(*this);
        for(int i = 0; i < n; i++) p[i] *= fact[i];
        p = p.rev();
        P bs(n, T(1));
        for(int i = 1; i < n; i++) bs[i] = bs[i - 1] * c * rfact[i] * fact[i - 1];
        p = (p * bs).pre(n);
        p = p.rev();
        for(int i = 0; i < n; i++) p[i] *= rfact[i];
        return p;
    }
};

//FPSの挙動 refer: https://ei1333.github.io/library/math/fps/formal-power-series-friendly-ntt.hpp

using mint = modint998244353;

template< typename Mint >
using FPS = FormalPowerSeriesFriendlyNTT< Mint >;
using fps = FPS<mint>;