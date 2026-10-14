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

template <class S, S (*op)(S, S), S (*e)()> struct segtree {
  public:
    segtree() : segtree(0) {}
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
    explicit segtree(const std::vector<S>& v) : _n((int)v.size()) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};

template <class S, S (*op)(S, S), S (*e)(), class Coordinate> class RangeTree {
    int n;
    using Pt = std::pair<Coordinate, Coordinate>;
    std::vector<Pt> _pts;
    std::vector<std::vector<Pt>> _range2yxs;
    std::vector<segtree<S, op, e>> segtrees;
    void _set(int v, Pt p, S val) {
        auto i = std::distance(
            _range2yxs[v].begin(),
            std::lower_bound(_range2yxs[v].begin(), _range2yxs[v].end(), Pt{p.second, p.first}));
        segtrees[v].set(i, val);
    }
    void _add(int v, Pt p, S val) {
        auto i = std::distance(
            _range2yxs[v].begin(),
            std::lower_bound(_range2yxs[v].begin(), _range2yxs[v].end(), Pt{p.second, p.first}));
        segtrees[v].set(i, op(segtrees[v].get(i), val));
    }
    S _prod(int v, Coordinate yl, Coordinate yr) const {
        auto comp = [&](const Pt &l, const Pt &r) { return l.first < r.first; };
        auto il = std::distance(
            _range2yxs[v].begin(),
            std::lower_bound(_range2yxs[v].begin(), _range2yxs[v].end(), Pt{yl, yl}, comp));
        auto ir = std::distance(
            _range2yxs[v].begin(),
            std::lower_bound(_range2yxs[v].begin(), _range2yxs[v].end(), Pt{yr, yr}, comp));
        return segtrees[v].prod(il, ir);
    }

public:
    RangeTree() = default;
    void add_point(Coordinate x, Coordinate y) noexcept { _pts.emplace_back(x, y); }
    void build() {
        std::sort(_pts.begin(), _pts.end());
        _pts.erase(std::unique(_pts.begin(), _pts.end()), _pts.end());
        n = _pts.size();

        _range2yxs.resize(n * 2);
        for (int i = 0; i < n; i++) _range2yxs[n + i] = {{_pts[i].second, _pts[i].first}};
        for (int i = n - 1; i > 0; i--) {
            auto &lch = _range2yxs[i * 2];
            auto &rch = _range2yxs[i * 2 + 1];
            std::merge(
                lch.begin(), lch.end(), rch.begin(), rch.end(), std::back_inserter(_range2yxs[i]));
            _range2yxs[i].erase(
                std::unique(_range2yxs[i].begin(), _range2yxs[i].end()), _range2yxs[i].end());
        }
        for (const auto &v : _range2yxs) segtrees.emplace_back(v.size());
    }
    void set(Coordinate x, Coordinate y, S val) {
        int i = std::distance(_pts.begin(), std::lower_bound(_pts.begin(), _pts.end(), Pt{x, y}));
        assert(i < n and _pts[i] == std::make_pair(x, y));
        for (i += n; i; i >>= 1) _set(i, {x, y}, val);
    }
    void add(Coordinate x, Coordinate y, S val) {
        int i = std::distance(_pts.begin(), std::lower_bound(_pts.begin(), _pts.end(), Pt{x, y}));
        assert(i < n and _pts[i] == std::make_pair(x, y));
        for (i += n; i; i >>= 1) _add(i, {x, y}, val);
    }
    // l, d, r, u [xl, xr) * [yl, yr)
    S prod(Coordinate xl, Coordinate yl, Coordinate xr, Coordinate yr) const {
        auto comp = [](const Pt &l, const Pt &r) { return l.first < r.first; };
        int l = n + std::distance(_pts.begin(),
                                  std::lower_bound(_pts.begin(), _pts.end(), Pt{xl, yr}, comp));
        int r = n + std::distance(_pts.begin(),
                                  std::lower_bound(_pts.begin(), _pts.end(), Pt{xr, yr}, comp));
        S ret = e();
        while (l < r) {
            if (l & 1) ret = op(ret, _prod(l++, yl, yr));
            if (r & 1) ret = op(ret, _prod(--r, yl, yr));
            l >>= 1, r >>= 1;
        }
        return ret;
    }
    S get(Coordinate x, Coordinate y) const { return prod(x, x + 1, y, y + 1); }
};

// 更新・加算を行う可能性のある点を全てadd_pointする
// bulidする
// 以上の2ステップが終わった後に、set, add, prod, getが行える

using S = long long;
S op(S x, S y) {
    return x+y;
}
S e() {
    return 0;
}
//ref: https://hitonanode.github.io/cplib-cpp/segmenttree/rangetree.hpp.html