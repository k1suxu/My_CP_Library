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
} // namespace internal

template <class S, S (*op)(S, S), S (*e)()>
struct segtree {
  public:
    segtree() : segtree(0) {}
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
    explicit segtree(const std::vector<S> &v) : _n((int)v.size()) {
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

    template <bool (*f)(S)>
    int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F>
    int max_right(int l, F f) const {
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

    template <bool (*f)(S)>
    int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F>
    int min_left(int r, F f) const {
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

template <class S, S (*op)(S, S), S (*e)(), class Coordinate, bool binary_search_X = false, bool binary_search_Y = false>
class RangeTree {
  private:
    using Pt = std::pair<Coordinate, Coordinate>;

    class AxisTree {
      private:
        std::vector<Pt> points_;
        std::vector<Coordinate> primary_coordinates_;
        std::vector<std::vector<Pt>> node_points_;
        std::vector<segtree<S, op, e>> segtrees_;

        int n_ = 0;
        int size_ = 1;

        int lower_primary(Coordinate x) const {
            return static_cast<int>(std::lower_bound(primary_coordinates_.begin(), primary_coordinates_.end(), x) - primary_coordinates_.begin());
        }

        int lower_secondary(int v, Coordinate y) const {
            auto comp = [](const Pt &lhs, const Pt &rhs) { return lhs.first < rhs.first; };
            return static_cast<int>(std::lower_bound(node_points_[v].begin(), node_points_[v].end(), Pt{y, y}, comp) - node_points_[v].begin());
        }

        int point_index(int v, Coordinate primary, Coordinate secondary) const {
            Pt target{secondary, primary};
            auto it = std::lower_bound(node_points_[v].begin(), node_points_[v].end(), target);

            assert(it != node_points_[v].end());
            assert(*it == target);

            return static_cast<int>(it - node_points_[v].begin());
        }

        S node_prod(int v, Coordinate secondary_l, Coordinate secondary_r) const {
            int l = lower_secondary(v, secondary_l);
            int r = lower_secondary(v, secondary_r);
            return segtrees_[v].prod(l, r);
        }

        template <class F>
        int max_right_dfs(int v, int nl, int nr, int ql, int qr, Coordinate secondary_l, Coordinate secondary_r, S &accumulated, const F &f) const {
            if (nr <= ql || qr <= nl) {
                return -1;
            }

            if (ql <= nl && nr <= qr) {
                S next = op(accumulated, node_prod(v, secondary_l, secondary_r));
                if (f(next)) {
                    accumulated = next;
                    return -1;
                }
                if (nr - nl == 1) {
                    return nl;
                }
            }

            int mid = (nl + nr) >> 1;
            int result = max_right_dfs(v << 1, nl, mid, ql, qr, secondary_l, secondary_r, accumulated, f);

            if (result != -1) {
                return result;
            }

            return max_right_dfs(v << 1 | 1, mid, nr, ql, qr, secondary_l, secondary_r, accumulated, f);
        }

        template <class F>
        int min_left_dfs(int v, int nl, int nr, int ql, int qr, Coordinate secondary_l, Coordinate secondary_r, S &accumulated, const F &f) const {
            if (nr <= ql || qr <= nl) {
                return -1;
            }

            if (ql <= nl && nr <= qr) {
                S next = op(node_prod(v, secondary_l, secondary_r), accumulated);
                if (f(next)) {
                    accumulated = next;
                    return -1;
                }
                if (nr - nl == 1) {
                    return nl + 1;
                }
            }

            int mid = (nl + nr) >> 1;
            int result = min_left_dfs(v << 1 | 1, mid, nr, ql, qr, secondary_l, secondary_r, accumulated, f);

            if (result != -1) {
                return result;
            }

            return min_left_dfs(v << 1, nl, mid, ql, qr, secondary_l, secondary_r, accumulated, f);
        }

      public:
        void clear_points() { points_.clear(); }

        void add_point(Coordinate primary, Coordinate secondary) { points_.emplace_back(primary, secondary); }

        void build() {
            std::sort(points_.begin(), points_.end());
            points_.erase(std::unique(points_.begin(), points_.end()), points_.end());
            primary_coordinates_.clear();
            primary_coordinates_.reserve(points_.size());

            for (const auto &[primary, secondary] : points_) {
                primary_coordinates_.push_back(primary);
            }

            std::sort(primary_coordinates_.begin(), primary_coordinates_.end());
            primary_coordinates_.erase(std::unique(primary_coordinates_.begin(), primary_coordinates_.end()), primary_coordinates_.end());
            n_ = static_cast<int>(primary_coordinates_.size());

            size_ = 1;
            while (size_ < n_) {
                size_ <<= 1;
            }

            node_points_.assign(2 * size_, {});

            for (const auto &[primary, secondary] : points_) {
                int i = lower_primary(primary);
                node_points_[size_ + i].emplace_back(secondary, primary);
            }

            for (int v = size_; v < 2 * size_; ++v) {
                auto &points = node_points_[v];
                std::sort(points.begin(), points.end());
                points.erase(std::unique(points.begin(), points.end()), points.end());
            }

            for (int v = size_ - 1; v >= 1; --v) {
                const auto &left = node_points_[v << 1];
                const auto &right = node_points_[v << 1 | 1];
                auto &current = node_points_[v];

                current.reserve(left.size() + right.size());
                std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(current));
            }

            segtrees_.clear();
            segtrees_.reserve(2 * size_);

            for (const auto &points : node_points_) {
                segtrees_.emplace_back(static_cast<int>(points.size()));
            }
        }

        void set(Coordinate primary, Coordinate secondary, const S &value) {
            assert(std::binary_search(points_.begin(), points_.end(), Pt{primary, secondary}));

            int i = lower_primary(primary);

            for (int v = size_ + i; v > 0; v >>= 1) {
                int j = point_index(v, primary, secondary);
                segtrees_[v].set(j, value);
            }
        }

        void add(Coordinate primary, Coordinate secondary, const S &value) {
            assert(std::binary_search(points_.begin(), points_.end(), Pt{primary, secondary}));

            int i = lower_primary(primary);

            for (int v = size_ + i; v > 0; v >>= 1) {
                int j = point_index(v, primary, secondary);

                segtrees_[v].set(j, op(segtrees_[v].get(j), value));
            }
        }

        S prod(Coordinate primary_l, Coordinate primary_r, Coordinate secondary_l, Coordinate secondary_r) const {
            assert(primary_l <= primary_r);
            assert(secondary_l <= secondary_r);

            int l = size_ + lower_primary(primary_l);
            int r = size_ + lower_primary(primary_r);

            S left_result = e();
            S right_result = e();

            while (l < r) {
                if (l & 1) {
                    left_result = op(left_result, node_prod(l, secondary_l, secondary_r));
                    ++l;
                }

                if (r & 1) {
                    --r;
                    right_result = op(node_prod(r, secondary_l, secondary_r), right_result);
                }

                l >>= 1;
                r >>= 1;
            }

            return op(left_result, right_result);
        }

        S get(Coordinate primary, Coordinate secondary) const {
            assert(std::binary_search(points_.begin(), points_.end(), Pt{primary, secondary}));

            int i = lower_primary(primary);
            int v = size_ + i;
            int j = point_index(v, primary, secondary);

            return segtrees_[v].get(j);
        }

        template <class F>
        int max_right_index(int l, int r, Coordinate secondary_l, Coordinate secondary_r, F f) const {
            assert(0 <= l);
            assert(l <= r);
            assert(r <= n_);
            assert(f(e()));

            if (l == r) {
                return r;
            }

            S accumulated = e();
            int result = max_right_dfs(1, 0, size_, l, r, secondary_l, secondary_r, accumulated, f);

            return result == -1 ? r : result;
        }

        template <class F>
        int min_left_index(int l, int r, Coordinate secondary_l, Coordinate secondary_r, F f) const {
            assert(0 <= l);
            assert(l <= r);
            assert(r <= n_);
            assert(f(e()));

            if (l == r) {
                return l;
            }

            S accumulated = e();
            int result = min_left_dfs(1, 0, size_, l, r, secondary_l, secondary_r, accumulated, f);

            return result == -1 ? l : result;
        }

        template <class F>
        Coordinate max_right(Coordinate primary_l, Coordinate primary_r, Coordinate secondary_l, Coordinate secondary_r, F f) const {
            static_assert(std::is_integral_v<Coordinate>, "Coordinate max_right requires an integral Coordinate type");

            assert(primary_l <= primary_r);
            assert(secondary_l <= secondary_r);
            assert(f(e()));

            int l = lower_primary(primary_l);
            int r = lower_primary(primary_r);

            int result = max_right_index(l, r, secondary_l, secondary_r, f);

            if (result == r) {
                return primary_r;
            }

            return primary_coordinates_[result];
        }

        template <class F>
        Coordinate min_left(Coordinate primary_l, Coordinate primary_r, Coordinate secondary_l, Coordinate secondary_r, F f) const {
            static_assert(std::is_integral_v<Coordinate>, "Coordinate min_left requires an integral Coordinate type");

            assert(primary_l <= primary_r);
            assert(secondary_l <= secondary_r);
            assert(f(e()));

            int l = lower_primary(primary_l);
            int r = lower_primary(primary_r);

            int result = min_left_index(l, r, secondary_l, secondary_r, f);

            if (result == l) {
                return primary_l;
            }

            return static_cast<Coordinate>(primary_coordinates_[result - 1] + 1);
        }
    };

    std::vector<Pt> points_;
    AxisTree x_tree_;
    AxisTree y_tree_;

  public:
    static constexpr bool supports_binary_search_x = binary_search_X;
    static constexpr bool supports_binary_search_y = binary_search_Y;
    static constexpr bool use_x_tree = binary_search_X || !binary_search_Y;

    static constexpr bool use_y_tree = binary_search_Y;

    RangeTree() = default;

    void add_point(Coordinate x, Coordinate y) { points_.emplace_back(x, y); }

    void build() {
        std::sort(points_.begin(), points_.end());
        points_.erase(std::unique(points_.begin(), points_.end()), points_.end());

        if constexpr (use_x_tree) {
            x_tree_.clear_points();

            for (const auto &[x, y] : points_) {
                x_tree_.add_point(x, y);
            }

            x_tree_.build();
        }

        if constexpr (use_y_tree) {
            y_tree_.clear_points();

            for (const auto &[x, y] : points_) {
                y_tree_.add_point(y, x);
            }

            y_tree_.build();
        }
    }

    void set(Coordinate x, Coordinate y, const S &value) {
        if constexpr (use_x_tree) {
            x_tree_.set(x, y, value);
        }

        if constexpr (use_y_tree) {
            y_tree_.set(y, x, value);
        }
    }

    void add(Coordinate x, Coordinate y, const S &value) {
        if constexpr (use_x_tree) {
            x_tree_.add(x, y, value);
        }

        if constexpr (use_y_tree) {
            y_tree_.add(y, x, value);
        }
    }

    S prod(Coordinate xl, Coordinate xr, Coordinate yl, Coordinate yr) const {
        if constexpr (use_x_tree) {
            return x_tree_.prod(xl, xr, yl, yr);
        } else {
            return y_tree_.prod(yl, yr, xl, xr);
        }
    }

    S get(Coordinate x, Coordinate y) const {
        if constexpr (use_x_tree) {
            return x_tree_.get(x, y);
        } else {
            return y_tree_.get(y, x);
        }
    }

    template <class F>
    Coordinate max_right_x(Coordinate xl, Coordinate xr, Coordinate yl, Coordinate yr, F f) const {
        if constexpr (!binary_search_X) {
            static_assert(binary_search_X, "RangeTree::max_right_x requires "
                                           "binary_search_X == true");

            return Coordinate{};
        } else {
            return x_tree_.max_right(xl, xr, yl, yr, f);
        }
    }

    template <class F>
    Coordinate min_left_x(Coordinate xl, Coordinate xr, Coordinate yl, Coordinate yr, F f) const {
        if constexpr (!binary_search_X) {
            static_assert(binary_search_X, "RangeTree::min_left_x requires "
                                           "binary_search_X == true");

            return Coordinate{};
        } else {
            return x_tree_.min_left(xl, xr, yl, yr, f);
        }
    }

    template <class F>
    Coordinate max_right_y(Coordinate xl, Coordinate xr, Coordinate yl, Coordinate yr, F f) const {
        if constexpr (!binary_search_Y) {
            static_assert(binary_search_Y, "RangeTree::max_right_y requires "
                                           "binary_search_Y == true");

            return Coordinate{};
        } else {
            return y_tree_.max_right(yl, yr, xl, xr, f);
        }
    }

    template <class F>
    Coordinate min_left_y(Coordinate xl, Coordinate xr, Coordinate yl, Coordinate yr, F f) const {
        if constexpr (!binary_search_Y) {
            static_assert(binary_search_Y, "RangeTree::min_left_y requires "
                                           "binary_search_Y == true");

            return Coordinate{};
        } else {
            return y_tree_.min_left(yl, yr, xl, xr, f);
        }
    }
};

// 更新・加算を行う可能性のある点を全てadd_pointする
// bulidする
// 以上の2ステップが終わった後に、set, add, prod, getが行える

using S = pii;
S op(S x, S y) {
    return {x.first + y.first, x.second + y.second};
}
S e() {
    return {0, 0};
}