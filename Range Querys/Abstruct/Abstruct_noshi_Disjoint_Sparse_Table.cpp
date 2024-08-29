template <class S, S (*op)(S, S), S (*e)()> class disjoint_sparse_table {
    std::vector<std::vector<S>> t;

    int size() const { return t[0].size() - 2; }

public:
    disjoint_sparse_table(const std::vector<S> &v) : t() {
        const int n = v.size() + 2;
        const int h = 32 - __builtin_clz(n - 1);
        t.assign(h, std::vector<S>(n, e()));
        for (int k = 1; k < h; k++) {
            auto &s = t[k];
            const int w = 1 << k;
            for (int i = w; i < n; i += w * 2) {
                for (int j = i - 1; j > i - w; j--)
                s[j - 1] = op(v[j - 1], s[j]);
                const int m = std::min(i + w - 1, n - 1);
                for (int j = i; j < m; j++)
                s[j + 1] = op(s[j], v[j - 1]);
            }
        }
    }

    S get(int p) const {
        assert(0 <= p && p < size());
        return prod(p, p + 1);
    }
    
    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        r++;
        const auto &s = t[31 - __builtin_clz(l ^ r)];
        return op(s[l], s[r]);
    }
};