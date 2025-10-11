random_device seed_gen;
mt19937 engine(seed_gen());
uniform_int_distribution<int> dist_treap(0, (1 << 30) - 1);

struct S {
	int len_parity;
	ll v0, v1;
};

S op(S a, S b) {

	if (a.len_parity == 0) {
		return S{a.len_parity ^ b.len_parity, a.v0 + b.v0, a.v1 + b.v1};
	}else{
		return S{a.len_parity ^ b.len_parity, a.v0 + b.v1, a.v1 + b.v0};
	}
}

S e() {
	return S{0, 0, 0};
}

struct treap {
	struct node {
		node *lch, *rch;
		int par;
		ll val;
		S monoid;
		S all_prod;
		int cnt;
		node(ll v, int p):
			lch(nullptr), rch(nullptr),
			par(p), val(v), cnt(1) {
				monoid = S{
					1, v, 0
				};
				all_prod = monoid;
			}
	};

	int calc_cnt(node* t){
		if (t == nullptr) return 0;
		return t->cnt;
	}

	S calc_monoid(node* t) {
		if (t == nullptr) return e();
		return t->all_prod;
	}

	node* update(node* t){
		t->cnt = calc_cnt(t->lch) + calc_cnt(t->rch) + 1;
		t->all_prod = op(
			op(calc_monoid(t->lch), t->monoid),
			calc_monoid(t->rch)
		);
		return t;
	}

	node* merge(node* a, node* b){
		if (a == nullptr || b == nullptr){
			if (a == nullptr) return b;
			return a;
		}

		if (a->par > b->par){
			a->rch = merge(a->rch, b);
			return update(a);
		}else{
			b->lch = merge(a, b->lch);
			return update(b);
		}
	}

	pair<node*, node*> split(node* t, int k){
		if (t == nullptr) return pair(nullptr, nullptr);
		assert(0 <= k && k <= calc_cnt(t));

		if (calc_cnt(t->lch) >= k){
			pair<node*, node*> ret = split(t->lch, k);
			t->lch = ret.second;
			return pair(ret.first, update(t));
		}else{
			pair<node*, node*> ret = split(t->rch, k - calc_cnt(t->lch) - 1);
			t->rch = ret.first;
			return pair(update(t), ret.second);
		}
	}

	node* insert(node* t, int k, ll v){
		return insert(t, k, v, dist_treap(engine));
	}

	node* insert(node* t, int k, ll v, int p){
		pair<node*, node*> tar = split(t, k);
		node* var = new node(v, p);
		return merge(merge(tar.first, var), tar.second);
	}

	node* erase(node* t, int k){
		assert(0 <= k && k < calc_cnt(t));
		pair<node*, node*> spl1 = split(t, k + 1);
		pair<node*, node*> spl2 = split(spl1.first, k);
		delete spl2.second;
		return merge(spl2.first, spl1.second);
	}

	// returns the number of elements less than v
	int lower_bound(node* t, ll v){
		if (t == nullptr) return 0;

		if (v <= t->val){
			return lower_bound(t->lch, v);
		}else{
			return calc_cnt(t->lch) + 1 + lower_bound(t->rch, v);
		}
	}

	node* insert(node* t, ll v){
		return insert(t, lower_bound(t, v), v);
	}

	// returns val
	ll get(node* t, int k){
		assert(t != nullptr);
		if (k < calc_cnt(t->lch)){
			return get(t->lch, k);
		}else if(calc_cnt(t->lch) == k){
			return t->val;
		}else{
			return get(t->rch, k - calc_cnt(t->lch) - 1);
		}
	}
};

// ref: https://atcoder.jp/contests/abc403/submissions/65272665
// 上のモノイドと、node(ll v, int p)のところを修正すればよい
