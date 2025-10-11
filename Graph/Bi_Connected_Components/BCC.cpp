template <typename T = int>
struct Edge {
  int from, to;
  T cost;
  int idx;

  Edge() = default;

  Edge(int from, int to, T cost = 1, int idx = -1)
      : from(from), to(to), cost(cost), idx(idx) {}

  operator int() const { return to; }
};

template <typename T = int>
struct Graph {
  vector<vector<Edge<T> > > g;
  int es;

  Graph() = default;

  explicit Graph(int n) : g(n), es(0) {}

  size_t size() const { return g.size(); }

  void add_directed_edge(int from, int to, T cost = 1) {
    g[from].emplace_back(from, to, cost, es++);
  }

  void add_edge(int from, int to, T cost = 1) {
    g[from].emplace_back(from, to, cost, es);
    g[to].emplace_back(to, from, cost, es++);
  }

  void read(int M, int padding = -1, bool weighted = false,
            bool directed = false) {
    for (int i = 0; i < M; i++) {
      int a, b;
      cin >> a >> b;
      a += padding;
      b += padding;
      T c = T(1);
      if (weighted) cin >> c;
      if (directed)
        add_directed_edge(a, b, c);
      else
        add_edge(a, b, c);
    }
  }

  inline vector<Edge<T> > &operator[](const int &k) { return g[k]; }

  inline const vector<Edge<T> > &operator[](const int &k) const { return g[k]; }
};

template <typename T = int>
using Edges = vector<Edge<T> >;

template <typename T = int>
struct LowLink : Graph<T> {
 public:
  using Graph<T>::Graph;
  vector<int> ord, low, articulation;
  vector<Edge<T> > bridge;
  using Graph<T>::g;

  virtual void build() {
    used.assign(g.size(), 0);
    ord.assign(g.size(), 0);
    low.assign(g.size(), 0);
    int k = 0;
    for (int i = 0; i < (int)g.size(); i++) {
      if (!used[i]) k = dfs(i, k, -1);
    }
  }

  explicit LowLink(const Graph<T> &g) : Graph<T>(g) {}

 private:
  vector<int> used;

  int dfs(int idx, int k, int par) {
    used[idx] = true;
    ord[idx] = k++;
    low[idx] = ord[idx];
    bool is_articulation = false, beet = false;
    int cnt = 0;
    for (auto &to : g[idx]) {
      if (to == par && !exchange(beet, true)) {
        continue;
      }
      if (!used[to]) {
        ++cnt;
        k = dfs(to, k, idx);
        low[idx] = min(low[idx], low[to]);
        is_articulation |= par >= 0 && low[to] >= ord[idx];
        if (ord[idx] < low[to]) bridge.emplace_back(to);
      } else {
        low[idx] = min(low[idx], ord[to]);
      }
    }
    is_articulation |= par == -1 && cnt > 1;
    if (is_articulation) articulation.push_back(idx);
    return k;
  }
};

template <typename T = int>
struct BiConnectedComponents : LowLink<T> {
 public:
  using LowLink<T>::LowLink;
  using LowLink<T>::g;
  using LowLink<T>::ord;
  using LowLink<T>::low;

  vector<vector<Edge<T> > > bc; // bc[i]: グループiに属する辺の集合

  void build() override {
    LowLink<T>::build();
    used.assign(g.size(), 0);
    for (int i = 0; i < (int)used.size(); i++) {
      if (!used[i]) dfs(i, -1);
    }
  }

  explicit BiConnectedComponents(const Graph<T> &g) : LowLink<T>(g) {}

 private:
  vector<int> used;
  vector<Edge<T> > tmp;

  void dfs(int idx, int par) {
    used[idx] = true;
    bool beet = false;
    for (auto &to : g[idx]) {
      if (to == par && !exchange(beet, true)) continue;
      if (!used[to] || ord[to] < ord[idx]) {
        tmp.emplace_back(to);
      }
      if (!used[to]) {
        dfs(to, idx);
        if (low[to] >= ord[idx]) {
          bc.emplace_back();
          for (;;) {
            auto e = tmp.back();
            bc.back().emplace_back(e);
            tmp.pop_back();
            if (e.idx == to.idx) break;
          }
        }
      }
    }
  }
};