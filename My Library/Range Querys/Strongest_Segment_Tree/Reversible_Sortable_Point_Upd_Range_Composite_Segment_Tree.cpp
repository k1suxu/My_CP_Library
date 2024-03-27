//ref: https://judge.yosupo.jp/submission/154167
template<typename Key, typename monoid, typename Val>
struct segment_tree_range_sort{
private:
    struct node{
        Key k, kmin, kmax;
        Val val, sum;
        bool reverse;
        int sz_all, sz_inner;
        unsigned int p;
        node *inner_l, *inner_r, *outer_l, *outer_r;
        node(Key k, Val val, unsigned int p): k(k), kmin(k), kmax(k), val(val), sum(val), reverse(false),
        sz_all(1), sz_inner(1), p(p), inner_l(nullptr), inner_r(nullptr), outer_l(nullptr), outer_r(nullptr){}
    };
    using pn = std::pair<node*, node*>;
    using pt = std::tuple<node*, node*, node*>;
    static unsigned int xor128(){
        static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned int t = (x ^ (x << 11));
        x = y, y = z, z = w;
        return (w = (w ^ (w >> 19)) ^ (t ^ ( t >> 8)));
    }
    void update(node *v){
        v->sz_inner = 1;
        v->sum = monoid::template id<Val>();
        v->kmin = v->kmax = v->k;
        if(v->outer_l) v->sum = v->outer_l->sum;
        if(v->inner_l){
            v->sz_inner += v->inner_l->sz_inner;
            v->sum = monoid::template merge<Val>(v->sum, v->inner_l->sum);
            v->kmin = std::min(v->kmin, v->inner_l->kmin);
            v->kmax = std::max(v->kmax, v->inner_l->kmax);
        }
        v->sum = monoid::template merge<Val>(v->sum, v->val);
        if(v->inner_r){
            v->sz_inner += v->inner_r->sz_inner;
            v->sum = monoid::template merge<Val>(v->sum, v->inner_r->sum);
            v->kmin = std::min(v->kmin, v->inner_r->kmin);
            v->kmax = std::max(v->kmax, v->inner_r->kmax);
        }
        if(v->outer_r) v->sum = monoid::template merge<Val>(v->sum, v->outer_r->sum);
        v->sz_all = v->sz_inner + (v->outer_l ? v->outer_l->sz_all : 0) + (v->outer_r ? v->outer_r->sz_all : 0);
    }
    int size(node *v){
        return v ? v->sz_all : 0;
    }
    node *make_node(Key k, Val val){
        return new node(k, val, xor128());
    }
    void reverse(node *v){
        std::swap(v->inner_l, v->inner_r);
        std::swap(v->outer_l, v->outer_r);
        v->sum = monoid::template reverse<Val>(v->sum);
        v->reverse ^= 1;
    }
    void push_down(node *v){
        if(!v->reverse) return;
        if(v->inner_l) reverse(v->inner_l);
        if(v->inner_r) reverse(v->inner_r);
        if(v->outer_l) reverse(v->outer_l);
        if(v->outer_r) reverse(v->outer_r);
        v->reverse = false;
    }
    node *insert(node *v, node *n, int k){
        if(!v) return n;
        push_down(v);
        int szl = size(v->outer_l);
        if(v->p < n->p){
            pn s = split_outer(v, k);
            n->outer_l = s.first;
            n->outer_r = s.second;
            update(n);
            return n;
        }else if(szl <= k && k < szl + v->sz_inner){
            pn s = split_outer(v, k);
            s.first = insert(s.first, n, k);
            update(s.first);
            return merge_outer(s.first, s.second);
        }else if(k < szl){
            v->outer_l = insert(v->outer_l, n, k);
        }else{
            v->outer_r = insert(v->outer_r, n, k - szl - v->sz_inner);
        }
        update(v);
        return v;
    }
    node *merge_inner(node *a, node *b){
        if(!a || !b) return !a ? b : a;
        push_down(a), push_down(b);
        if(a->p > b->p){
            a->inner_r = merge_inner(a->inner_r, b);
            update(a);
            return a;
        }else{
            b->inner_l = merge_inner(a, b->inner_l);
            update(b);
            return b;
        }
    }
    node *merge_outer(node *a, node *b){
        if(!a || !b) return !a ? b : a;
        push_down(a), push_down(b);
        if(a->p > b->p){
            a->outer_r = merge_outer(a->outer_r, b);
            update(a);
            return a;
        }else{
            b->outer_l = merge_outer(a, b->outer_l);
            update(b);
            return b;
        }
    }
    pt cut_inner(node *v, int k){
        if(!v) return {nullptr, nullptr, nullptr};
        push_down(v);
        int szl = size(v->inner_l);
        if(k <= szl){
            if(k == szl){
                pt res = {v->inner_l, v, v->inner_r};
                v->inner_l = v->inner_r = nullptr;
                update(v);
                return res;
            }
            auto [a, b, c] = cut_inner(v->inner_l, k);
            v->inner_l = c;
            update(v);
            return {a, b, v};
        }else{
            auto [a, b, c] = cut_inner(v->inner_r, k - szl - 1);
            v->inner_r = a;
            update(v);
            return {v, b, c};
        }
    }
    pt cut_outer(node *v, int k){
        if(!v) return {nullptr, nullptr, nullptr};
        push_down(v);
        int szl = size(v->outer_l);
        int szr = szl + v->sz_inner;
        if(k < szl){
            auto [a, b, c] = cut_outer(v->outer_l, k);
            v->outer_l = c;
            update(v);
            return {a, b, v};
        }else if(szr <= k){
            auto [a, b, c] = cut_outer(v->outer_r, k - szr);
            v->outer_r = a;
            update(v);
            return {v, b, c};
        }else{
            node *tmp_l = v->outer_l, *tmp_r = v->outer_r;
            v->outer_l = v->outer_r = nullptr;
            auto [a, b, c] = cut_inner(v, k - szl);
            a = merge_outer(tmp_l, a);
            c = merge_outer(c, tmp_r);
            return {a, b, c};
        }
    }
    pn split_inner(node *v, int k){
        if(!v) return {nullptr, nullptr};
        push_down(v);
        int szl = size(v->inner_l);
        if(k <= szl){
            pn s = split_inner(v->inner_l, k);
            v->inner_l = s.second;
            update(v);
            return {s.first, v};
        }else{
            pn s = split_inner(v->inner_r, k - szl - 1);
            v->inner_r = s.first;
            update(v);
            return {v, s.second};
        }
    }
    void update_range_inner(node *v, int l, int r){
        if(!v) return;
        if(l == 0 && r == v->sz_all){
            // propagate
            return;
        }
        push_down(v);
        int szl = size(v->inner_l);
        int szr = szl + 1;
        if(l < szl){
            if(r <= szl){
                update_range_inner(v->inner_l, l, r);
                update(v);
                return;
            }
            update_range_inner(v->inner_l, l, szl);
            l = szl;
        }
        if(szr < r){
            if(szr <= l){
                update_range_inner(v->inner_r, l - szr, r - szr);
                update(v);
                return;
            }
            update_range_inner(v->inner_r, 0, r - szr);
            r = szr;
        }
        if(l != r){
            // apply
        }
        update(v);
    }
    void update_range_outer(node *v, int l, int r){
        if(!v) return;
        if(l == 0 && r == v->sz_all){
            // propagate
            return;
        }
        push_down(v);
        int szl = size(v->outer_l);
        int szr = szl + v->sz_inner;
        if(l < szl){
            if(r <= szl){
                update_range_outer(v->outer_l, l, r);
                update(v);
                return;
            }
            update_range_outer(v->outer_l, l, szl);
            l = szl;
        }
        if(szr < r){
            if(szr <= l){
                update_range_outer(v->outer_r, l - szr, r - szr);
                update(v);
                return;
            }
            update_range_outer(v->outer_r, 0, r - szr);
            r = szr;
        }
        if(l != r){
            // apply
        }
        update(v);
    }
    Val prod_range_inner(node *v, int l, int r){
        if(!v) return monoid::template id<Val>();
        if(l == 0 && r == v->sz_all) return v->sum;
        push_down(v);
        int szl = size(v->inner_l);
        int szr = szl + 1;
        Val left_q = monoid::template id<Val>(), right_q = left_q;
        if(l < szl){
            if(r <= szl) return prod_range_inner(v->inner_l, l, r);
            left_q = prod_range_inner(v->inner_l, l, szl);
            l = szl;
        }
        if(szr < r){
            if(szr <= l) return prod_range_inner(v->inner_r, l - szr, r - szr);
            right_q = prod_range_inner(v->inner_r, 0, r - szr);
            r = szr;
        }
        Val res = (l == r ? monoid::template id<Val>() : v->val);
        res = monoid::template merge<Val>(left_q, res);
        res = monoid::template merge<Val>(res, right_q);
        return res;
    }
    Val prod_range_outer(node *v, int l, int r){
        if(!v) return monoid::template id<Val>();
        if(l == 0 && r == v->sz_all) return v->sum;
        push_down(v);
        int szl = size(v->outer_l);
        int szr = szl + v->sz_inner;
        Val left_q = monoid::template id<Val>(), right_q = left_q;
        if(l < szl){
            if(r <= szl) return prod_range_outer(v->outer_l, l, r);
            left_q = prod_range_outer(v->outer_l, l, szl);
            l = szl;
        }
        if(szr < r){
            if(szr <= l) return prod_range_outer(v->outer_r, l - szr, r - szr);
            right_q = prod_range_outer(v->outer_r, 0, r - szr);
            r = szr;
        }
        Val res = (l == r ? monoid::template id<Val>() : prod_range_inner(v, l - szl, r - szl));
        res = monoid::template merge<Val>(left_q, res);
        res = monoid::template merge<Val>(res, right_q);
        return res;
    }
    pn split_outer(node *v, int k){
        if(!v) return {nullptr, nullptr};
        push_down(v);
        int szl = size(v->outer_l);
        int szr = szl + v->sz_inner;
        if(k < szl){
            pn s = split_outer(v->outer_l, k);
            v->outer_l = s.second;
            update(v);
            return {s.first, v};
        }else if(szr <= k){
            pn s = split_outer(v->outer_r, k - szr);
            v->outer_r = s.first;
            update(v);
            return {v, s.second};
        }else{
            node *tmp_l = v->outer_l, *tmp_r = v->outer_r;
            v->outer_l = v->outer_r = nullptr;
            pn s = split_inner(v, k - szl);
            s.first = merge_outer(tmp_l, s.first);
            s.second = merge_outer(s.second, tmp_r);
            return {s.first, s.second};
        }
    }
    pt split_range_outer(node *v, int l, int r){
        auto [a, b] = split_outer(v, l);
        auto [bb, c] = split_outer(b, r - l);
        return {a, bb, c};
    }
    pn split_key(node *v, Key k){
        if(!v) return {nullptr, nullptr};
        if(k < v->kmin) return {nullptr, v};
        else if(v->kmax <= k) return {v, nullptr};
        push_down(v);
        if(k < v->k){
            pn s = split_key(v->inner_l, k);
            v->inner_l = s.second;
            update(v);
            return {s.first, v};
        }else{
            pn s = split_key(v->inner_r, k);
            v->inner_r = s.first;
            update(v);
            return {v, s.second};
        }
    }
    node *merge_compress(node *a, node *b){
        if(!a || !b) return !a ? b : a;
        push_down(a), push_down(b);
        if(a->p < b->p) std::swap(a, b);
        if(a->k <= b->kmin){
            a->inner_r = merge_compress(a->inner_r, b);
        }else if(b->kmax <= a->k){
            a->inner_l = merge_compress(a->inner_l, b);
        }else{
            auto [bl, br] = split_key(b, a->k);
            a->inner_l = merge_compress(a->inner_l, bl);
            a->inner_r = merge_compress(a->inner_r, br);
        }
        update(a);
        return a;
    }
    node *sort_inner(node *v){
        if(!v) return nullptr;
        node *tmp_l = v->outer_l, *tmp_r = v->outer_r;
        push_down(v);
        v->outer_l = v->outer_r = nullptr;
        node *res = sort_inner(tmp_l);
        if((v->inner_l && v->k < v->inner_l->kmax) || (v->inner_r && v->inner_r->kmin < v->k)) reverse(v);
        res = merge_compress(res, v);
        res = merge_compress(res, sort_inner(tmp_r));
        return res;
    }
    void enumerate_inner(node *v, std::vector<std::pair<Key, Val>> &res){
        if(!v) return;
        push_down(v);
        if(v->inner_l) enumerate_inner(v->inner_l, res);
        res.push_back({v->key, v->val});
        if(v->inner_r) enumerate_inner(v->inner_r, res);
    }
    void enumerate_outer(node *v, std::vector<std::pair<Key, Val>> &res){
        if(!v) return;
        push_down(v);
        if(v->outer_l) enumerate_outer(v->outer_l, res);
        enumerate_inner(v, res);
        if(v->outer_r) enumerate_outer(v->outer_r, res);
    }
    void p_satisfy(node *v){
        if(!v->outer_l){
            if(!v->outer_r || v->p > v->outer_r->p) return;
            std::swap(v->p, v->outer_r->p);
            p_satisfy(v->outer_r);
        }else if(!v->outer_r){
            if(v->p > v->outer_l->p) return;
            std::swap(v->p, v->outer_l->p);
            p_satisfy(v->outer_l);
        }else{
            if(v->outer_l->p > v->outer_r->p){
                if(v->p > v->outer_l->p) return;
                std::swap(v->p, v->outer_l->p);
                p_satisfy(v->outer_l);
            }else{
                if(v->p > v->outer_r->p) return;
                std::swap(v->p, v->outer_r->p);
                p_satisfy(v->outer_r);
            }
        }
    }
    node *build(int l, int r, std::vector<std::pair<Key, Val>> &v){
        int mid = (l + r) / 2;
        node *u = make_node(v[mid].first, v[mid].second);

        if(l < mid) u->outer_l = build(l, mid, v);
        else u->outer_l = nullptr;
        if(mid + 1 < r) u->outer_r = build(mid + 1, r, v);
        else u->outer_r = nullptr;

        p_satisfy(u);
        update(u);
        return u;
    }
    node *root;
public:
    segment_tree_range_sort(): root(nullptr){}
    segment_tree_range_sort(std::vector<std::pair<Key, Val>> &v): root(v.empty() ? nullptr : build(0, v.size(), v)){}

    void insert(int k, Key key, Val x){
        root = insert(root, make_node(key, x), k);
    }
    void erase(int k){
        auto [a, b, c] = cut_outer(root, k);
        root = merge_outer(a, c);
    }
    void set(int k, Key key, Val x){
        auto [a, b, c] = cut_outer(root, k);
        b->k = key;
        b->val = x;
        update(b);
        root = merge_outer(a, merge_outer(b, c));
    }
    void update(int k, Val x){
        auto [a, b, c] = cut_outer(root, k, k);
        b->val = monoid::template merge<Val>(b->val, x);
        update(b);
        root = merge_outer(a, merge_outer(b, c));
    }
    Val prod(int l, int r){
        if(r == l) return monoid::template id<Val>();
        return prod_range_outer(root, l, r);
    }
    Val all_prod(){
        assert(root);
        return root->sum;
    }
    void reverse(int l, int r){
        if(r == l) return;
        auto [a, b, c] = split_range_outer(root, l, r);
        reverse(b);
        root = merge_outer(a, merge_outer(b, c));
    }
    void sort_ascending(int l, int r){
        if(r == l) return;
        auto [a, b, c] = split_range_outer(root, l, r);
        b = sort_inner(b);
        root = merge_outer(a, merge_outer(b, c));
    }
    void sort_descending(int l, int r){
        if(r == l) return;
        auto [a, b, c] = split_range_outer(root, l, r);
        b = sort_inner(b);
        reverse(b);
        root = merge_outer(a, merge_outer(b, c));
    }
    std::vector<std::pair<Key, Val>> to_vector(){
        std::vector<std::pair<Key, Val>> res;
        enumerate_outer(root, res);
        return res;
    }
};

// submit status: https://judge.yosupo.jp/submission/195470