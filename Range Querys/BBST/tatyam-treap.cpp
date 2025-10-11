ll rnd() {
    static ll x = 0xe3036d86dfb4e29c;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}
struct T {
    bool len;
    ll sum0, sum1;
};
T operator+(const T& a, const T& b) {
    if (a.len) return {!b.len, a.sum0 + b.sum1, a.sum1 + b.sum0};
    return {b.len, a.sum0 + b.sum0, a.sum1 + b.sum1};
}
struct Node {
    Node* l;
    Node* r;
    ll pri, val;
    T sum;
};
Node* recalc(Node* t) {
    t->sum = {1, t->val, 0};
    if (t->l) t->sum = t->l->sum + t->sum;
    if (t->r) t->sum = t->sum + t->r->sum;
    return t;
}
using Tree = Node*;
Tree merge(Tree l, Tree r) {
    if (l == nullptr) return r;
    if (r == nullptr) return l;
    if (l->pri > r->pri) {
        l->r = merge(l->r, r);
        return recalc(l);
    } else {
        r->sum = l->sum + r->sum;
        r->l = merge(l, r->l);
        return recalc(r);
    }
}
pair<Tree, Tree> split_by_value(Tree t, ll k) {
    if (t == nullptr) return {nullptr, nullptr};
    if (k <= 0) return {nullptr, t};
    if (t->val < k) {
        auto [l, r] = split_by_value(t->r, k);
        if (r) {
            t->r = l;
            recalc(t);
        }
        return {t, r};
    } else {
        auto [l, r] = split_by_value(t->l, k);
        if (l) {
            t->l = r;
            recalc(t);
        }
        return {l, t};
    }
}
Tree insert(Tree t, ll val) {
    auto [l, r] = split_by_value(t, val);
    Tree mid = new Node{nullptr, nullptr, rnd(), val, {1, val, 0}};
    return merge(merge(l, mid), r);
}

// ref: https://atcoder.jp/contests/abc403/submissions/65270783

