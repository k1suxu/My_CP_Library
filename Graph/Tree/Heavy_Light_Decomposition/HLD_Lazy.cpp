//ref: https://nyaannyaan.github.io/library/tree/heavy-light-decomposition.hpp.html
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

template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct lazy_segtree {
  public:
    lazy_segtree() : lazy_segtree(0) {}
    explicit lazy_segtree(int n) : lazy_segtree(std::vector<S>(n, e())) {}
    explicit lazy_segtree(const std::vector<S>& v) : _n(v.size()) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        lz = std::vector<F>(size, id());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return e();

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        S sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = mapping(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template <bool (*g)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template <class G> int max_right(int l, G g) {
        assert(0 <= l && l <= _n);
        assert(g(e()));
        if (l == _n) return _n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) {
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

    template <bool (*g)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template <class G> int min_left(int r, G g) {
        assert(0 <= r && r <= _n);
        assert(g(e()));
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm))) {
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
    std::vector<F> lz;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if (k < size) lz[k] = composition(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
};

template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*ident)()>
struct Noncommutative_HeavyLightDecomposition {
 private:
  void dfs_sz(int cur) {
    size[cur] = 1;
    for (auto& dst : g[cur]) {
      if (dst == par[cur]) {
        if (g[cur].size() >= 2 && dst == g[cur][0])
          swap(g[cur][0], g[cur][1]);
        else
          continue;
      }
      depth[dst] = depth[cur] + 1;
      par[dst] = cur;
      dfs_sz(dst);
      size[cur] += size[dst];
      if (size[dst] > size[g[cur][0]]) {
        swap(dst, g[cur][0]);
      }
    }
  }

  void dfs_hld(int cur) {
    down[cur] = id++;
    for (auto dst : g[cur]) {
      if (dst == par[cur]) continue;
      nxt[dst] = (dst == g[cur][0] ? nxt[cur] : dst);
      dfs_hld(dst);
    }
    up[cur] = id;
  }

  // [u, v)
  vector<pair<int, int>> ascend(int u, int v) const {
    vector<pair<int, int>> res;
    while (nxt[u] != nxt[v]) {
      res.emplace_back(down[u], down[nxt[u]]);
      u = par[nxt[u]];
    }
    if (u != v) res.emplace_back(down[u], down[v] + 1);
    return res;
  }

  // (u, v]
  vector<pair<int, int>> descend(int u, int v) const {
    if (u == v) return {};
    if (nxt[u] == nxt[v]) return {{down[u] + 1, down[v]}};
    auto res = descend(u, par[nxt[v]]);
    res.emplace_back(down[nxt[v]], down[v]);
    return res;
  }

 public:
    using Graph = vector<vector<int>>;

    Graph g;
    int n, id;
    vector<int> size, depth, down, up, nxt, par;
    lazy_segtree<S, op, e, F, mapping, composition, ident> seg, rev_seg;

    Noncommutative_HeavyLightDecomposition() = default;
    Noncommutative_HeavyLightDecomposition(int n) : n(n), g(n) {}
    Noncommutative_HeavyLightDecomposition(Graph _g, int root = 0) : n(_g.size()), g(_g), id(0), size(g.size(), 0), depth(g.size(), 0), down(g.size(), -1), up(g.size(), -1), nxt(g.size(), root), par(g.size(), root), seg(g.size()), rev_seg(g.size()) {
        dfs_sz(root);
        dfs_hld(root);
    }
    Noncommutative_HeavyLightDecomposition(Graph _g, int root, vector<S> vertex_weight) : n(_g.size()), g(_g), id(0), size(g.size(), 0), depth(g.size(), 0), down(g.size(), -1), up(g.size(), -1), nxt(g.size(), root), par(g.size(), root), seg(g.size()), rev_seg(g.size()) {
        dfs_sz(root);
        dfs_hld(root);
        // for(int i = 0; i < g.size(); i++) cout << down[i] << " "; cout << "\n";
        // for(int i = 0; i < g.size(); i++) cout << up[i] << " "; cout << "\n";
        for(int i = 0; i < g.size(); i++) vertex_update(i, vertex_weight[i]);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void rebuild(int root) {
        id = 0;
        size.assign(n, 0);
        depth.assign(n, 0);
        down.assign(n, -1);
        up.assign(n, -1);
        nxt.assign(n, root);
        par.assign(n, root);
        seg = lazy_segtree<S, op, e, F, mapping, composition, ident>(n);
        rev_seg = lazy_segtree<S, op, e, F, mapping, composition, ident>(n);
        dfs_sz(root);
        dfs_hld(root);
    }

    //以降はrebuildまたはconstructorでbuildのどちらかが行われていないと正常に作動しない。

    pair<int, int> idx(int i) const { return make_pair(down[i], up[i]); }

    S path_prod(int u, int v) {
        S ans(e());
        int l = lca(u, v);
        for (pair<int, int> elem : ascend(u, l)) {
            int s = elem.first + 1, t = elem.second;
            ans = op(ans, s > t ? rev_seg.prod(n-s, n-t) : seg.prod(s, t));
        }
        ans = op(ans, seg.prod(down[l], down[l] + 1));
        for (pair<int, int> elem : descend(l, v)) {
            int s = elem.first, t = elem.second + 1;
            ans = op(ans, s > t ? rev_seg.prod(n-s, n-t) : seg.prod(s, t));
        }
        return ans;
    }

    void path_apply(int u, int v, F f) {
        int l = lca(u, v);
        for (pair<int, int> elem : ascend(u, l)) {
            int s = elem.first + 1, t = elem.second;
            rev_seg.apply(min(n-s, n-t), max(n-s, n-t), f);
            seg.apply(min(s, t), max(s, t), f);
        }
        seg.apply(down[l], down[l]+1, f);
        rev_seg.apply(n-down[l]-1, n-down[l], f);
        for (pair<int, int> elem : descend(l, v)) {
            int s = elem.first, t = elem.second + 1;
            rev_seg.apply(min(n-s, n-t), max(n-s, n-t), f);
            seg.apply(min(s, t), max(s, t), f);
        }
    }

    S subtree_prod(int u, bool include = true) {
        return seg.prod(down[u] + !include, up[u]);
    }

    void subtree_apply(int u, F f, bool include = true) {
        seg.apply(down[u] + !include, up[u], f);
    }

    int lca(int a, int b) {
        while (nxt[a] != nxt[b]) {
            if (down[a] < down[b]) swap(a, b);
            a = par[nxt[a]];
        }
        return depth[a] < depth[b] ? a : b;
    }

    int dist(int a, int b) { return depth[a] + depth[b] - depth[lca(a, b)] * 2; }

    void vertex_update(int u, S val) {
        int now_id = idx(u).first;
        seg.set(now_id, val);
        rev_seg.set(n-1-now_id, val);
    }
    S get(int u) {
        return seg.get(idx(u).first);
    }
};

//edge_update, edge_add
//la, jump

using S = int;
S op(S l, S r) {
    return min(l, r);
}
S e() {
    return INF;
}

// affine合成のような可換律が成り立たない演算(モノイド)も上手くできる。

template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
using HLD = Noncommutative_HeavyLightDecomposition<S, op, e, F, mapping, composition, id>;

/**************************/
/****    Undebugged    ****/
/**************************/