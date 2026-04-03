// https://nyaannyaan.github.io/library/tree/static-top-tree-vertex-based.hpp.html

template <typename T>
struct edge {
  int src, to;
  T cost;

  edge(int _to, T _cost) : src(-1), to(_to), cost(_cost) {}
  edge(int _src, int _to, T _cost) : src(_src), to(_to), cost(_cost) {}

  edge &operator=(const int &x) {
    to = x;
    return *this;
  }

  operator int() const { return to; }
};
template <typename T>
using Edges = vector<edge<T>>;
template <typename T>
using WeightedGraph = vector<Edges<T>>;
using UnweightedGraph = vector<vector<int>>;

// Input of (Unweighted) Graph
UnweightedGraph graph(int N, int M = -1, bool is_directed = false,
                      bool is_1origin = true) {
  UnweightedGraph g(N);
  if (M == -1) M = N - 1;
  for (int _ = 0; _ < M; _++) {
    int x, y;
    cin >> x >> y;
    if (is_1origin) x--, y--;
    g[x].push_back(y);
    if (!is_directed) g[y].push_back(x);
  }
  return g;
}

// Input of Weighted Graph
template <typename T>
WeightedGraph<T> wgraph(int N, int M = -1, bool is_directed = false,
                        bool is_1origin = true) {
  WeightedGraph<T> g(N);
  if (M == -1) M = N - 1;
  for (int _ = 0; _ < M; _++) {
    int x, y;
    cin >> x >> y;
    T c;
    cin >> c;
    if (is_1origin) x--, y--;
    g[x].emplace_back(x, y, c);
    if (!is_directed) g[y].emplace_back(y, x, c);
  }
  return g;
}

// Input of Edges
template <typename T>
Edges<T> esgraph([[maybe_unused]] int N, int M, int is_weighted = true,
                 bool is_1origin = true) {
  Edges<T> es;
  for (int _ = 0; _ < M; _++) {
    int x, y;
    cin >> x >> y;
    T c;
    if (is_weighted)
      cin >> c;
    else
      c = 1;
    if (is_1origin) x--, y--;
    es.emplace_back(x, y, c);
  }
  return es;
}

// Input of Adjacency Matrix
template <typename T>
vector<vector<T>> adjgraph(int N, int M, T INF, int is_weighted = true,
                           bool is_directed = false, bool is_1origin = true) {
  vector<vector<T>> d(N, vector<T>(N, INF));
  for (int _ = 0; _ < M; _++) {
    int x, y;
    cin >> x >> y;
    T c;
    if (is_weighted)
      cin >> c;
    else
      c = 1;
    if (is_1origin) x--, y--;
    d[x][y] = c;
    if (!is_directed) d[y][x] = c;
  }
  return d;
}

template <typename T>
struct has_cost {
 private:
  template <typename U>
  static auto confirm(U u) -> decltype(u.cost, std::true_type());
  static auto confirm(...) -> std::false_type;

 public:
  enum : bool { value = decltype(confirm(std::declval<T>()))::value };
};

template <typename T>
vector<vector<T>> inverse_tree(const vector<vector<T>>& g) {
  int N = (int)g.size();
  vector<vector<T>> rg(N);
  for (int i = 0; i < N; i++) {
    for (auto& e : g[i]) {
      if constexpr (is_same<T, int>::value) {
        rg[e].push_back(i);
      } else if constexpr (has_cost<T>::value) {
        rg[e].emplace_back(e.to, i, e.cost);
      } else {
        assert(0);
      }
    }
  }
  return rg;
}

template <typename T>
vector<vector<T>> rooted_tree(const vector<vector<T>>& g, int root = 0) {
  int N = (int)g.size();
  vector<vector<T>> rg(N);
  vector<char> v(N, false);
  v[root] = true;
  queue<int> que;
  que.emplace(root);
  while (!que.empty()) {
    auto p = que.front();
    que.pop();
    for (auto& e : g[p]) {
      if (v[e] == false) {
        v[e] = true;
        que.push(e);
        rg[p].push_back(e);
      }
    }
  }
  return rg;
}

template <typename G>
struct HeavyLightDecomposition {
 private:
  void dfs_sz(int cur) {
    size[cur] = 1;
    for (auto& dst : g[cur]) {
      if (dst == par[cur]) {
        if (g[cur].size() >= 2 && (int)(dst) == (int)(g[cur][0]))
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
      nxt[dst] = ((int)(dst) == (int)(g[cur][0]) ? nxt[cur] : (int)(dst));
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
  G& g;
  int root, id;
  vector<int> size, depth, down, up, nxt, par;
  HeavyLightDecomposition(G& _g, int _root = 0)
      : g(_g),
        root(_root),
        id(0),
        size(g.size(), 0),
        depth(g.size(), 0),
        down(g.size(), -1),
        up(g.size(), -1),
        nxt(g.size(), root),
        par(g.size(), root) {
    dfs_sz(root);
    dfs_hld(root);
  }

  pair<int, int> idx(int i) const { return make_pair(down[i], up[i]); }

  template <typename F>
  void path_query(int u, int v, bool vertex, const F& f) {
    int l = lca(u, v);
    for (auto&& [a, b] : ascend(u, l)) {
      int s = a + 1, t = b;
      s > t ? f(t, s) : f(s, t);
    }
    if (vertex) f(down[l], down[l] + 1);
    for (auto&& [a, b] : descend(l, v)) {
      int s = a, t = b + 1;
      s > t ? f(t, s) : f(s, t);
    }
  }

  template <typename F>
  void path_noncommutative_query(int u, int v, bool vertex, const F& f) {
    int l = lca(u, v);
    for (auto&& [a, b] : ascend(u, l)) f(a + 1, b);
    if (vertex) f(down[l], down[l] + 1);
    for (auto&& [a, b] : descend(l, v)) f(a, b + 1);
  }

  template <typename F>
  void subtree_query(int u, bool vertex, const F& f) {
    f(down[u] + (int)(!vertex), up[u]);
  }

  int lca(int a, int b) {
    while (nxt[a] != nxt[b]) {
      if (down[a] < down[b]) swap(a, b);
      a = par[nxt[a]];
    }
    return depth[a] < depth[b] ? a : b;
  }

  int dist(int a, int b) { return depth[a] + depth[b] - depth[lca(a, b)] * 2; }
};

namespace StaticTopTreeVertexBasedImpl {

enum Type { Vertex, Compress, Rake, Add_Edge, Add_Vertex };

template <typename G>
struct StaticTopTreeVertexBased {
  const HeavyLightDecomposition<G>& hld;
  vector<vector<int>> g;
  int root;     // 元の木の root
  int tt_root;  // top tree の root
  vector<int> P, L, R;
  vector<Type> T;

  StaticTopTreeVertexBased(const HeavyLightDecomposition<G>& _hld) : hld(_hld) {
    root = hld.root;
    g = rooted_tree(hld.g, root);
    int n = g.size();
    P.resize(n, -1), L.resize(n, -1), R.resize(n, -1);
    T.resize(n, Type::Vertex);
    build();
  }

 private:
  int add(int l, int r, Type t) {
    if (t == Type::Compress or t == Type::Rake) {
      assert(l != -1 and r != -1);
    }
    if (t == Type::Add_Edge) {
      assert(l != -1 and r == -1);
    }
    assert(t != Type::Vertex and t != Type::Add_Vertex);
    int k = P.size();
    P.push_back(-1), L.push_back(l), R.push_back(r), T.push_back(t);
    if (l != -1) P[l] = k;
    if (r != -1) P[r] = k;
    return k;
  }
  int add2(int k, int l, int r, Type t) {
    assert(k < (int)g.size());
    assert(t == Type::Vertex or t == Type::Add_Vertex);
    if (t == Type::Vertex) {
      assert(l == -1 and r == -1);
    } else {
      assert(l != -1 and r == -1);
    }
    P[k] = -1, L[k] = l, R[k] = r, T[k] = t;
    if (l != -1) P[l] = k;
    if (r != -1) P[r] = k;
    return k;
  }
  pair<int, int> merge(const vector<pair<int, int>>& a, Type t) {
    assert(!a.empty());
    if (a.size() == 1) return a[0];
    int sum_s = 0;
    for (auto& [_, s] : a) sum_s += s;
    vector<pair<int, int>> b, c;
    for (auto& [i, s] : a) {
      (sum_s > s ? b : c).emplace_back(i, s);
      sum_s -= s * 2;
    }
    auto [i, si] = merge(b, t);
    auto [j, sj] = merge(c, t);
    return {add(i, j, t), si + sj};
  }
  pair<int, int> compress(int i) {
    vector<pair<int, int>> chs;
    while (true) {
      chs.push_back(add_vertex(i));
      if (g[i].empty()) break;
      i = g[i][0];
    }
    return merge(chs, Type::Compress);
  }
  pair<int, int> rake(int i) {
    vector<pair<int, int>> chs;
    for (int j = 1; j < (int)g[i].size(); j++) chs.push_back(add_edge(g[i][j]));
    if (chs.empty()) return {-1, 0};
    return merge(chs, Type::Rake);
  }
  pair<int, int> add_edge(int i) {
    auto [j, sj] = compress(i);
    return {add(j, -1, Type::Add_Edge), sj};
  }
  pair<int, int> add_vertex(int i) {
    auto [j, sj] = rake(i);
    return {add2(i, j, -1, j == -1 ? Type::Vertex : Type::Add_Vertex), sj + 1};
  }
  void build() {
    auto [i, n] = compress(root);
    assert((int)g.size() == n);
    tt_root = i;
  }
};

template <typename G, typename Path, typename Point, typename Vertex,
          typename Compress, typename Rake, typename Add_edge,
          typename Add_vertex>
struct DPonStaticTopTreeVertexBased {
  const StaticTopTreeVertexBased<G> tt;
  vector<Path> path;
  vector<Point> point;
  const Vertex vertex;
  const Compress compress;
  const Rake rake;
  const Add_edge add_edge;
  const Add_vertex add_vertex;

  DPonStaticTopTreeVertexBased(const HeavyLightDecomposition<G>& hld,
                               const Vertex& _vertex, const Compress& _compress,
                               const Rake& _rake, const Add_edge& _add_edge,
                               const Add_vertex& _add_vertex)
      : tt(hld),
        vertex(_vertex),
        compress(_compress),
        rake(_rake),
        add_edge(_add_edge),
        add_vertex(_add_vertex) {
    int n = tt.P.size();
    path.resize(n), point.resize(n);
    dfs(tt.tt_root);
  }

  Path get() { return path[tt.tt_root]; }
  void update(int k) {
    while (k != -1) _update(k), k = tt.P[k];
  }

 private:
  void _update(int k) {
    if (tt.T[k] == Type::Vertex) {
      path[k] = vertex(k);
    } else if (tt.T[k] == Type::Compress) {
      path[k] = compress(path[tt.L[k]], path[tt.R[k]]);
    } else if (tt.T[k] == Type::Rake) {
      point[k] = rake(point[tt.L[k]], point[tt.R[k]]);
    } else if (tt.T[k] == Type::Add_Edge) {
      point[k] = add_edge(path[tt.L[k]]);
    } else {
      path[k] = add_vertex(point[tt.L[k]], k);
    }
  }

  void dfs(int k) {
    if (tt.L[k] != -1) dfs(tt.L[k]);
    if (tt.R[k] != -1) dfs(tt.R[k]);
    _update(k);
  }
};

}  // namespace StaticTopTreeVertexBasedImpl

using StaticTopTreeVertexBasedImpl::DPonStaticTopTreeVertexBased;
using StaticTopTreeVertexBasedImpl::StaticTopTreeVertexBased;