template<class T>
struct VectorPool {
    vector<T> pool;
    vector<T*> stock;
    int ptr;

    VectorPool() = default;

    VectorPool(int sz) : pool(sz), stock(sz) {}

    inline T *alloc() { return stock[--ptr]; }

    inline void free(T *t) { stock[ptr++] = t; }

    void clear() {
        ptr = (int) pool.size();
        for(int i = 0; i < pool.size(); i++) stock[i] = &pool[i];
    }
};

template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct LazyRedBlackTree {
public:
    enum COLOR {
        BLACK, RED
    };

    struct Node {
        Node *l, *r;
        COLOR color;
        int level, size;
        S self_val, sum;
        F lazy;

        Node() {}

        Node(const S &k, const F &laz) :
            self_val(k), sum(k), l(nullptr), r(nullptr), color(BLACK), level(0), size(1), lazy(laz) {}

        Node(Node *l, Node *r, const S &k, const F &laz) :
            self_val(k), color(RED), l(l), r(r), lazy(laz) {}

        bool is_leaf() const {
            return l == nullptr;
        }
    };

private:
    Node *propagate(Node *t) {
        t = clone(t);
        if(t->lazy != id()) {
            if(t->is_leaf()) {
                t->self_val = mapping(t->self_val, t->lazy);
            } else {
                if(t->l) {
                    t->l = clone(t->l);
                    t->l->lazy = composition(t->l->lazy, t->lazy);
                    t->l->sum = mapping(t->l->sum, t->lazy);
                }
                if(t->r) {
                    t->r = clone(t->r);
                    t->r->lazy = composition(t->r->lazy, t->lazy);
                    t->r->sum = mapping(t->r->sum, t->lazy);
                }
            }
            t->lazy = id();
        }
        return update(t);
    }

    inline Node *alloc(Node *l, Node *r) {
        auto t = &(*pool.alloc() = Node(l, r, e(), id()));
        return update(t);
    }

    virtual Node *clone(Node *t) {
        return t;
    }

    Node *rotate(Node *t, bool b) {
        t = propagate(t);
        Node *s;
        if(b) {
            s = propagate(t->l);
            t->l = s->r;
            s->r = t;
        } else {
            s = propagate(t->r);
            t->r = s->l;
            s->l = t;
        }
        update(t);
        return update(s);
    }

    Node *submerge(Node *l, Node *r) {
        if(l->level < r->level) {
            r = propagate(r);
            Node *c = (r->l = submerge(l, r->l));
            if(r->color == BLACK && c->color == RED && c->l && c->l->color == RED) {
                r->color = RED;
                c->color = BLACK;
                if(r->r->color == BLACK) return rotate(r, true);
                r->r->color = BLACK;
            }
            return update(r);
        }
        if(l->level > r->level) {
            l = propagate(l);
            Node *c = (l->r = submerge(l->r, r));
            if(l->color == BLACK && c->color == RED && c->r && c->r->color == RED) {
                l->color = RED;
                c->color = BLACK;
                if(l->l->color == BLACK) return rotate(l, false);
                l->l->color = BLACK;
            }
            return update(l);
        }
        return alloc(l, r);
    }

    Node *build(int l, int r, const vector< S > &v) {
        if(l + 1 >= r) return alloc(v[l]);
        return merge(build(l, (l + r) >> 1, v), build((l + r) >> 1, r, v));
    }

    Node *update(Node *t) {
        t->size = get_size(t->l) + get_size(t->r) + t->is_leaf();
        t->level = t->is_leaf() ? 0 : t->l->level + (t->l->color == BLACK);
        t->sum = op(op(sum(t->l), t->self_val), sum(t->r));
        return t;
    }

    void dump(Node *r, typename vector<S>::iterator &it, F lazy) {
        if(r->lazy != id()) lazy = composition(lazy, r->lazy);
        if(r->is_leaf()) {
            *it++ = mapping(r->self_val, lazy);
            return;
        }
        dump(r->l, it, lazy);
        dump(r->r, it, lazy);
    }

    Node *merge(Node *l) {
        return l;
    }

public:

    VectorPool<Node> pool;

    LazyRedBlackTree(int sz) : pool(2*sz) { pool.clear(); }


    inline Node *alloc(const S &self_val) {
        return &(*pool.alloc() = Node(self_val, id()));
    }

    inline int get_size(const Node *t) { return t ? t->size : 0; }

    inline const S sum(const Node *t) { return t ? t->sum : e(); }

    pair<Node*, Node*> split(Node *t, int k) {
        if(!t) return {nullptr, nullptr};
        t = propagate(t);
        if(k == 0) return {nullptr, t};
        if(k >= get_size(t)) return {t, nullptr};
        Node *l = t->l, *r = t->r;
        pool.free(t);
        if(k < get_size(l)) {
            auto pp = split(l, k);
            return {pp.first, merge(pp.second, r)};
        }
        if(k > get_size(l)) {
            auto pp = split(r, k - get_size(l));
            return {merge(l, pp.first), pp.second};
        }
        return {l, r};
    }

    tuple<Node*, Node*, Node*> split3(Node *t, int a, int b) {
        auto x = split(t, a);
        auto y = split(x.second, b - a);
        return make_tuple(x.first, y.first, y.second);
    }

    template<typename ... Args>
    Node *merge(Node *l, Args ...rest) {
        Node *r = merge(rest...);
        if(!l || !r) return l ? l : r;
        Node *c = submerge(l, r);
        c->color = BLACK;
        return c;
    }

    Node *build(const vector<S> &v) {
        return build(0, (int) v.size(), v);
    }

    vector<S> dump(Node *r) {
        vector<S> v((size_t) get_size(r));
        auto it = begin(v);
        dump(r, it, id());
        return v;
    }

    string to_string(Node *r) {
        auto s = dump(r);
        string ret;
        for(int i = 0; i < s.size(); i++) {
            ret += std::to_string(s[i]);
            ret += ", ";
        }
        return ret;
    }

    void insert(Node *&t, int k, const S &v) {
        auto x = split(t, k);
        t = merge(merge(x.first, alloc(v)), x.second);
    }

    S erase(Node *&t, int k) {
        auto x = split(t, k);
        auto y = split(x.second, 1);
        auto v = y.first->self_val;
        pool.free(y.first);
        t = merge(x.first, y.second);
        return v;
    }

    S prod(Node *&t, int a, int b) {
        auto x = split(t, a);
        auto y = split(x.second, b - a);
        S ret = sum(y.first);
        t = merge(x.first, y.first, y.second);
        return ret;
    }

    void apply(Node *&t, int a, int b, const F &pp) {
        auto x = split(t, a);
        auto y = split(x.second, b - a);
        y.first->lazy = composition(y.first->lazy, pp);
        t = merge(x.first, propagate(y.first), y.second);
    }

    void set(Node *&t, int k, const S &x) {
        t = propagate(t);
        if(t->is_leaf()) {
            t->self_val = t->sum = x;
            return;
        }
        if(k < get_size(t->l)) set(t->l, k, x);
        else set(t->r, k - get_size(t->l), x);
        t = update(t);
    }

    void push_front(Node *&t, const S &v) {
        t = merge(alloc(v), t);
    }

    void push_back(Node *&t, const S &v) {
        t = merge(t, alloc(v));
    }

    S pop_front(Node *&t) {
        auto ret = split(t, 1);
        t = ret.second;
        return ret.first->self_val;
    }

    S pop_back(Node *&t) {
        auto ret = split(t, get_size(t) - 1);
        t = ret.first;
        return ret.second->self_val;
    }

    // f(op(s[0],...,s[r-1]))=true,f(op(s[0],...,s[r]))=falseとなる最小のr
    // fが単調の時、f(op(s[0],...,s[r-1]))=trueとなる最大のrと考えられる
    // BBST上の二分探索 (前から)
    // なぜかこっちはTLEしたりするからデバッグしたい」ところではある
    template<typename F>
    int binary_search_right(Node *t, const F &f, S lsum = e()) {
        if(!t) return 0;
        if(f(op(lsum, t->sum))) return get_size(t);
        if(!(t->r)) return binary_search_right(t->l, f, lsum);
        assert(t->l);
        if(f(op(op(lsum, t->l->sum), t->self_val))) return get_size(t->l) + binary_search_right(t->r, f, op(op(lsum, t->l->sum), t->self_val));
        if(f(op(lsum, t->l->sum))) return get_size(t->l);
        return binary_search_right(t->l, f, lsum);
    }
    // // f(op(s[l],...,s[n-1]))=true,f(op(s[l-1],...,s[n-1]))=falseとなる最大のl
    // // fが単調の時、f(op(s[l],...,s[n-1]))=trueとなる最小のlと考えられる
    // // BBST上の二分探索 (後ろから)
    template<typename F>
    int binary_search_left(Node *t, const F &f, S rsum = e()) {
        if(!t) return 0;
        if(f(op(t->sum, rsum))) return 0;
        if(!(t->r)) {
            if(!(t->l)) return 1;
            return binary_search_left(t->l, f, op(t->self_val, rsum));
        }
        assert(t->l);
        if(f(op(t->self_val, op(t->r->sum, rsum)))) return binary_search_left(t->l, f, op(t->self_val, op(t->r->sum, rsum)));
        if(f(op(t->r->sum, rsum))) return get_size(t->l);
        return get_size(t->l) + binary_search_left(t->r, f, rsum);
    }
};

//ref: https://ei1333.github.io/library/structure/bbst/lazy-red-black-tree.hpp