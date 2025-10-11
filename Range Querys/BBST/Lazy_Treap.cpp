template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S, int),
          F (*composition)(F, F),
          F (*id)()>
class ImplicitTreap {
    class xorshift {
        uint64_t x;

    public:
        xorshift() {
            mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
            x = rnd();
            for (int i = 0; i < 100; i++) {
                random();
            }
        }

        uint64_t random() {
            x = x ^ (x << 7);
            return x = x ^ (x >> 9);
        }
    } rnd;
    struct Node {
        S value, acc;
        F lazy;
        int priority, cnt;
        bool rev;
        Node *l, *r;
        Node(S value, int priority) : value(value), acc(e()), lazy(id()), priority(priority), cnt(1), rev(false), l(nullptr), r(nullptr) {}
    } *root = nullptr;
    using Tree = Node *;

    int cnt(Tree t) {
        return t ? t->cnt : 0;
    }

    S acc(Tree t) {
        return t ? t->acc : e();
    }

    void update_cnt(Tree t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
    }

    void update_acc(Tree t) {
        if (t) {
            t->acc = op(acc(t->l), op(t->value, acc(t->r)));
        }
    }

    void pushup(Tree t) {
        update_cnt(t), update_acc(t);
    }

    void pushdown(Tree t) {
        if (t && t->rev) {
            t->rev = false;
            swap(t->l, t->r);
            if (t->l) t->l->rev ^= 1;
            if (t->r) t->r->rev ^= 1;
        }
        if (t && t->lazy != id()) {
            if (t->l) {
                t->l->lazy = composition(t->l->lazy, t->lazy);
                t->l->acc = mapping(t->lazy, t->l->acc, cnt(t->l));
            }
            if (t->r) {
                t->r->lazy = composition(t->r->lazy, t->lazy);
                t->r->acc = mapping(t->lazy, t->r->acc, cnt(t->r));
            }
            t->value = mapping(t->lazy, t->value, 1);
            t->lazy = id();
        }
        pushup(t);
    }
    
    void split(Tree t, int key, Tree& l, Tree& r) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        pushdown(t);
        int implicit_key = cnt(t->l) + 1;
        if (key < implicit_key) {
            split(t->l, key, l, t->l), r = t;
        } else {
            split(t->r, key - implicit_key, t->r, r), l = t;
        }
        pushup(t);
    }
    
    void insert(Tree& t, int key, Tree item) {
        Tree t1, t2;
        split(t, key, t1, t2);
        merge(t1, t1, item);
        merge(t, t1, t2);
    }

    void merge(Tree& t, Tree l, Tree r) {
        pushdown(l);
        pushdown(r);
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->priority > r->priority) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        pushup(t);
    }
    
    void erase(Tree& t, int key) {
        Tree t1, t2, t3;
        split(t, key + 1, t1, t2);
        split(t1, key, t1, t3);
        merge(t, t1, t2);
    }

    void apply(Tree t, int l, int r, F x) {
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2 , t3);
        t2->lazy = composition(t2->lazy, x);
        t2->acc = mapping(x, t2->acc, cnt(t2));
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    S prod(Tree t, int l, int r) {
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        S ret = t2->acc;
        merge(t2, t2, t3);
        merge(t, t1, t2);
        return ret;
    }

    // [l, r)の中で左から何番目か
    int find(Tree t, S x, int offset, bool left = true) {
        if (op(t->acc, x) == x) {
            return -1;
        } else {
            if (left) {
                if (t->l && op(t->l->acc, x) != x) {
                    return find(t->l, x, offset, left);
                } else {
                    return (op(t->value, x) != x) ? offset + cnt(t->l) : find(t->r, x, offset + cnt(t->l) + 1, left);
                }
            } else {
                if (t->r && op(t->r->acc, x) != x) {
                    return find(t->r, x, offset + cnt(t->l) + 1, left);
                } else {
                    return (op(t->value, x) != x) ? offset + cnt(t->l) : find(t->l, x, offset, left);
                }
            }
        }
    }

    void reverse(Tree t, int l, int r) {
        if (l > r) return;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        t2->rev ^= 1;
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    // [l, r)の先頭がmになるようにシフトさせる。std::rotateと同じ仕様
    void rotate(Tree t, int l, int m, int r) {
        reverse(t, l, r);
        reverse(t, l, l + r - m);
        reverse(t, l + r - m, r);
    }

    void dump(Tree t) {
        if (!t) return;
        pushdown(t);
        dump(t->l);
        cout << t->value << " ";
        dump(t->r);
    }
    
public:
    ImplicitTreap() {}
    ImplicitTreap(vector<S> as) {
        ::reverse(as.begin(), as.end());
        for (S a : as) {
            insert(0, a);
        }
    }

    int size() {
        return cnt(root);
    }

    void insert(int pos, S x) {
        insert(root, pos, new Node(x, rnd.random()));
    }

    void apply(int l, int r, F x) {
        apply(root, l, r, x);
    }

    S prod(int l, int r) {
        return prod(root, l, r);
    }

    // 二分探索。[l, r)内のkでMonoid::op(tr[k], x) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    int find(int l, int r, S x, bool left = true) {
        Tree t1, t2, t3;
        split(root, l, t1, t2);
        split(t2, r - l, t2, t3);
        int ret = find(t2, x, l, left);
        merge(t2, t2, t3);
        merge(root, t1, t2);
        return ret;
    }

    void erase(int pos) {
        erase(root, pos);
    }

    void reverse(int l, int r) {
        reverse(root, l, r);
    }

    void rotate(int l, int m, int r) {
        rotate(root, l, m, r);
    }

    void dump() {
        dump(root);
        cout << endl;
    }

    S operator[](int pos) {
        Tree t1, t2, t3;
        split(root, pos + 1, t1, t2);
        split(t1, pos, t1, t3);
        S ret = t3->acc;
        merge(t1, t1, t3);
        merge(root, t1, t2);
        return ret;
    }
};

struct S {
    ll odd, even, len;
};
S op(S x, S y) {
    if (x.len % 2 == 0) {
        return {x.odd + y.odd, x.even + y.even, x.len + y.len};
    }
    return {x.odd + y.even, x.even + y.odd, x.len + y.len};
}
S e() {
    return {0, 0, 0};
}
// segtreeの時はこんな感じにする
using F = int;
S mapping(F f, S x, int sz) {
    return x;
}
F composition(F x, F y) { return 0; }
F id() { return 0; }
