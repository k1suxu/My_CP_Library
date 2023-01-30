template< typename T = int >
struct Edge {
  int from, to;
  T cost;
  int idx;

  Edge() = default;

  Edge(int from, int to, T cost = 1, int idx = -1) : from(from), to(to), cost(cost), idx(idx) {}

  operator int() const { return to; }
};

template< typename T = int >
struct Graph {
  vector< vector< Edge< T > > > g;
  int es;

  Graph() = default;

  explicit Graph(int n) : g(n), es(0) {}

  size_t size() const {
    return g.size();
  }

  void add_directed_edge(int from, int to, T cost = 1) {
    g[from].emplace_back(from, to, cost, es++);
  }

  void add_edge(int from, int to, T cost = 1) {
    g[from].emplace_back(from, to, cost, es);
    g[to].emplace_back(to, from, cost, es++);
  }

  void read(int M, int padding = -1, bool weighted = false, bool directed = false) {
    for(int i = 0; i < M; i++) {
      int a, b;
      cin >> a >> b;
      a += padding;
      b += padding;
      T c = T(1);
      if(weighted) cin >> c;
      if(directed) add_directed_edge(a, b, c);
      else add_edge(a, b, c);
    }
  }

  inline vector< Edge< T > > &operator[](const int &k) {
    return g[k];
  }

  inline const vector< Edge< T > > &operator[](const int &k) const {
    return g[k];
  }
};

template< typename T = int >
using Edges = vector< Edge< T > >;

template< typename T, typename E = T >
struct SkewHeap {
  using G = function< T(T, E) >;
  using H = function< E(E, E) >;
 
  struct Node {
    T key;
    E lazy;
    Node *l, *r;
  };
 
  const bool rev;
  const G g;
  const H h;
 
  SkewHeap(bool rev = false) : g([](const T &a, const E &b) { return a + b; }),
                               h([](const E &a, const E &b) { return a + b; }), rev(rev) {}
 
  SkewHeap(const G &g, const H &h, bool rev = false) : g(g), h(h), rev(rev) {}
 
  Node *propagate(Node *t) {
    if(t->lazy != 0) {
      if(t->l) t->l->lazy = h(t->l->lazy, t->lazy);
      if(t->r) t->r->lazy = h(t->r->lazy, t->lazy);
      t->key = g(t->key, t->lazy);
      t->lazy = 0;
    }
    return t;
  }
 
  Node *merge(Node *x, Node *y) {
    if(!x || !y) return x ? x : y;
    propagate(x), propagate(y);
    if((x->key > y->key) ^ rev) swap(x, y);
    x->r = merge(y, x->r);
    swap(x->l, x->r);
    return x;
  }
 
  void push(Node *&root, const T &key) {
    root = merge(root, new Node({key, 0, nullptr, nullptr}));
  }
 
  T top(Node *root) {
    return propagate(root)->key;
  }
 
  T pop(Node *&root) {
    T top = propagate(root)->key;
    auto *temp = root;
    root = merge(root->l, root->r);
    delete temp;
    return top;
  }
 
  bool empty(Node *root) const {
    return !root;
  }
 
  void add(Node *root, const E &lazy) {
    if(root) {
      root->lazy = h(root->lazy, lazy);
      propagate(root);
    }
  }
 
  Node *makeheap() {
    return nullptr;
  }
};

struct UnionFind {
    vector<int> r;
    UnionFind(int n) : r(n, -1) {}

    int find(int x) {
        if(r[x] < 0) return x;
        return r[x] = find(r[x]);
    }

    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if(x == y) return false;
        if(r[x] > r[y]) swap(x, y);
        r[x] += r[y];
        r[y] = x;
        return true;
    }
};

template<typename T>
struct minimum_spanning_tree
{
  using Pi = pair< T, int >;
  using Heap = SkewHeap< Pi, int >;
  using Node = typename Heap::Node;
  const Edges< T > &es;
  const int V;
  T INF;
 
  minimum_spanning_tree(const Edges< T > &es, int V) :
      INF(numeric_limits< T >::max()), es(es), V(V) {}
 
  T build(int start)
  {
    auto g = [](const Pi &a, const T &b) { return Pi(a.first + b, a.second); };
    auto h = [](const T &a, const T &b) { return a + b; };
    Heap heap(g, h);
    vector< Node * > heaps(V, heap.makeheap());
    for(auto &e : es) heap.push(heaps[e.to], {e.cost, e.from});
    UnionFind uf(V);
    vector< int > used(V, -1);
    used[start] = start;
 
    T ret = 0;
    for(int s = 0; s < V; s++) {
      stack< int > path;
      for(int u = s; used[u] < 0;) {
        path.push(u);
        used[u] = s;
        if(heap.empty(heaps[u])) return -1;
        auto p = heap.top(heaps[u]);
        ret += p.first;
        heap.add(heaps[u], -p.first);
        heap.pop(heaps[u]);
        int v = uf.find(p.second);
        if(used[v] == s) {
          int w;
          Node *nextheap = heap.makeheap();
          do {
            w = path.top();
            path.pop();
            nextheap = heap.merge(nextheap, heaps[w]);
          } while(uf.unite(v, w));
          heaps[uf.find(v)] = nextheap;
          used[uf.find(v)] = -1;
        }
        u = uf.find(v);
      }
    }
    return ret;
  }
};