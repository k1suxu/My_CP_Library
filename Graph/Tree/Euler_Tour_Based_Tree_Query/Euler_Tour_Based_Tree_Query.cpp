const long long ETBTQ_INF = 1e18;
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

    void set_all(S x) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = x;
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }
    void clear() {
        set_all(e());
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

using PII_FOR_SEG = pair<long long, long long>;

PII_FOR_SEG PII_FOR_SEG_op(PII_FOR_SEG x, PII_FOR_SEG  y) {
    return min(x, y);
}
PII_FOR_SEG PII_FOR_SEG_e() {
    return make_pair(ETBTQ_INF, ETBTQ_INF);
}

template<typename T>
struct Edge {
    int to, id;
    T info;

    Edge() = default;
    Edge(int to, int id, T info) : to(to), id(id), info(info) {}
};

//E_Type: edgeに持たせる情報の型
//V_Type: nodeに持たせる情報の型
template<typename E_Type, E_Type (*ET_op) (E_Type, E_Type), E_Type (*ET_e)(), E_Type (*ET_inv)(E_Type), E_Type(*ET_add)(E_Type, E_Type), typename V_Type, V_Type (*VT_op) (V_Type, V_Type), V_Type (*VT_e)(), V_Type (*VT_inv)(V_Type), V_Type(*VT_add)(V_Type, V_Type)>
struct Euler_Tour_Based_Tree_Query {
    int n, edge_id, t, last_root;
    vector<vector<Edge<E_Type>>> g;
    vector<V_Type> node;
    vector<int> in, out, down, up, par_edge_id, depth;
    vector<int> id;
    segtree<E_Type, ET_op, ET_e> E_seg, E_seg_subtree;
    segtree<V_Type, VT_op, VT_e> V_seg, V_seg_subtree;
    segtree<PII_FOR_SEG, PII_FOR_SEG_op, PII_FOR_SEG_e> lca_seg;
    
    // vector<pair<int, int>> inv;
    // //second: 0->in, 1->out, 2->down, 3->up

    Euler_Tour_Based_Tree_Query() = default;
    Euler_Tour_Based_Tree_Query(int n) : n(n), edge_id(0), g(n), node(n, VT_e()), E_seg(4*n), V_seg(4*n), E_seg_subtree(4*n), V_seg_subtree(4*n), lca_seg(4*n) {}
    Euler_Tour_Based_Tree_Query(int n, vector<V_Type> node) : n(n), edge_id(0), g(n), node(node), E_seg(4*n), V_seg(4*n), E_seg_subtree(4*n), V_seg_subtree(4*n), lca_seg(4*n) {}
    // Euler_Tour_Based_Tree_Query(int n, vector<vector<Edge<E_Type>>> g) : n(n), edge_id(0), g(g), node(n), E_seg(4*n), V_seg(4*n), E_seg_subtree(4*n), V_seg_subtree(4*n), lca_seg(4*n) {}
    // Euler_Tour_Based_Tree_Query(int n, vector<vector<Edge<E_Type>>> g, vector<V_Type> node) : n(n), edge_id(0), g(g), E_seg(4*n), V_seg(4*n), E_seg_subtree(4*n), V_seg_subtree(4*n), lca_seg(4*n) {}

    void add_edge(int u, int v, E_Type info) {
        g[u].emplace_back(v, edge_id, info);
        g[v].emplace_back(u, edge_id, info);
        edge_id++;
    }

    void dfs(int v, int p, int d) {
        depth[v] = d;
        V_seg.set(t, node[v]);
        V_seg_subtree.set(t, node[v]);
        lca_seg.set(t, make_pair(depth[v], v));
        in[v] = t++;
        for(auto e : g[v]) if(e.to != p) {
            E_seg.set(t, e.info);
            E_seg_subtree.set(t, e.info);
            lca_seg.set(t, {depth[v]+1, e.to});
            down[e.id] = t++;
            par_edge_id[e.to] = e.id;
            dfs(e.to, v, d+1);
            lca_seg.set(t, {depth[v], v});
            E_seg.set(t, ET_inv(e.info));
            up[e.id] = t++;
        }
        V_seg.set(t, VT_inv(node[v]));
        out[v] = t++;
    }

    void build(int root) {
        t = 0;
        last_root = root;
        in.assign(n, -1);
        out.assign(n, -1);
        down.assign(n-1, -1);
        up.assign(n-1, -1);
        par_edge_id.assign(n, -1);
        depth.assign(n, -1);
        E_seg.clear();
        V_seg.clear();
        E_seg_subtree.clear();
        V_seg_subtree.clear();
        lca_seg.clear();

        dfs(root, -1, 0);
    }

    //以降はbuild後にしか使えない。

    int lca(int u, int v) {
        return lca_seg.prod(min(in[u], in[v]), max(in[u], in[v])+1).second;
    }
    int lca(vector<int> vs) {
        int fr = *min_element(vs.begin(), vs.end(), [&](int u, int v) -> bool {return in[u] < in[v];});
        int ba = *min_element(vs.begin(), vs.end(), [&](int u, int v) -> bool {return in[u] > in[v];});
        return lca(fr, ba);
    }

    E_Type edge_path_query_root(int v) {
        return E_seg.prod(0, in[v]);
    }
    E_Type edge_path_query(int u, int v) {
        E_Type minus = ET_inv(edge_path_query_root(lca(u, v)));
        return ET_add(ET_add(ET_add(edge_path_query_root(u), edge_path_query_root(v)), minus), minus);
    }
    E_Type edge_subtree_query(int v) {
        return E_seg_subtree.prod(in[v], out[v]);
    }
    
    V_Type vertex_path_query_root(int v, bool included) {
        return V_seg.prod(0, in[v] + included);
    }
    V_Type vertex_path_query(int u, int v) {
        V_Type minus = VT_inv(vertex_path_query_root(lca(u, v), false));
        return VT_add(VT_add(VT_add(vertex_path_query_root(u, true), vertex_path_query_root(v, true)), minus), VT_add(minus, VT_inv(get_vertex(lca(u, v)))));
    }
    V_Type vertex_subtree_query(int v, bool included = true) {
        return V_seg_subtree.prod(in[v], out[v] + !included);
    }

    void edge_upd(int i, E_Type val) {
        E_seg.set(down[i], val);
        E_seg.set(up[i], ET_inv(val));
        E_seg_subtree.set(down[i], val);
    }
    void edge_add(int i, E_Type val) {
        E_Type new_edge = ET_add(get_edge(i), val);
        edge_upd(i, new_edge);
    }
    void par_edge_upd(int v, E_Type val) {
        if(v == last_root) return;
        edge_upd(par_edge_id[v], val);
    }
    E_Type get_edge(int i) {
        return E_seg.get(down[i]);
    }
    void vertex_upd(int v, V_Type val) {
        V_seg.set(in[v], val);
        V_seg.set(out[v], VT_inv(val));
        V_seg_subtree.set(in[v], val);
    }
    void vertex_add(int v, V_Type val) {
        V_Type new_node = VT_add(get_vertex(v), val);
        vertex_upd(v, new_node);
    }
    V_Type get_vertex(int v) {
        return V_seg.get(in[v]);
    }
};

using ET = long long;
ET ET_op(ET x, ET y) {
    return x + y;
}
ET ET_e() {
    return 0;
}
ET ET_inv(ET x) {
    return -x;
}
ET ET_add(ET x, ET y) {
    return x+y;
}

using VT = long long;
VT VT_op(VT x, VT y) {
    return x + y;
}
VT VT_e() {
    return 0;
}
VT VT_inv(VT x) {
    return -x;
}
VT VT_add(VT x, VT y) {
    return x+y;
}

using ETBTQ = Euler_Tour_Based_Tree_Query<ET, ET_op, ET_e, ET_inv, ET_add, VT, VT_op, VT_e, VT_inv, VT_add>;

/*
OK : add_edge https://atcoder.jp/contests/abc294/tasks/abc294_g
OK : LCA
   : multipoint LCA
OK : edge_path_query_root https://atcoder.jp/contests/abc294/tasks/abc294_g
OK : edge_path_query https://atcoder.jp/contests/abc294/tasks/abc294_g
   : edge_subtree_query
OK : vertex_path_query_root https://judge.yosupo.jp/problem/vertex_add_path_sum
OK : vertex_path_query https://judge.yosupo.jp/problem/vertex_add_path_sum
OK : vertex_subtree_query https://judge.yosupo.jp/problem/vertex_add_subtree_sum
OK : edge_upd https://atcoder.jp/contests/abc294/tasks/abc294_g
OK : edge_add https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D&lang=ja
OK : par_edge_upd https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D&lang=ja
OK : par_edge_add https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D&lang=ja
OK : get_edge https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D&lang=ja
OK : vertex_upd https://judge.yosupo.jp/problem/vertex_add_path_sum
OK : vertex_add https://judge.yosupo.jp/problem/vertex_add_path_sum
OK : get_vertex https://judge.yosupo.jp/problem/vertex_add_path_sum

https://judge.u-aizu.ac.jp/onlinejudge/submission.jsp#
Euler Tour LCA 00:19 
Doubling   LCA 00:11
*/
//subtree_apply(遅延セグメント木を作りたい)