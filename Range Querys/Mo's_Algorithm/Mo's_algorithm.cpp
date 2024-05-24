struct Mo
{
    vector<int> left, right, order;
    vector<bool> v;
    int width;
    int nl, nr, ptr;

    Mo(int n) : width((int) sqrt(n)), nl(0), nr(0), ptr(0), v(n) {}

    void insert(int l, int r) /* [l, r) */
    {
        left.push_back(l);
        right.push_back(r);
    }

    void build()
    {
        order.resize(left.size());
        iota(begin(order), end(order), 0);
        sort(begin(order), end(order), [&](int a, int b) {
            if(left[a] / width != left[b] / width) return left[a] < left[b];
            if(left[a] / width % 2 == 1) return right[a] > right[b];
            return right[a] < right[b];
        });
    }

    int process()
    {
        if(ptr == order.size()) return (-1);
        const auto id = order[ptr];
        while(nl > left[id]) add(--nl);
        while(nr < right[id]) add(nr++);
        while(nl < left[id]) del(nl++);
        while(nr > right[id]) del(--nr);
        return (order[ptr++]);
    }

    template<typename T, T (*get_ans)()>
    vector<T> solve() {
        int q = (int)left.size();
        build();
        vector<T> ans(q);
        for(int i = 0; i < q; ++i) {
            int idx = process();
            ans[idx] = get_ans();
        }
        return ans;
    }

    //TODO: redefine add and del
    //add calculates the value after IDX is taken into account.
    //del calculates the value after IDX is UNtaken into account.

    void add(int idx);
    void del(int idx);
};

//index iを加える
void Mo::add(int i) {
    
}
//index iを削除する
void Mo::del(int i) {
    
}
//現状でのanswer(aggregate result)を返す
bool get_ans() {
    
}







int n, q;
vector<int> a;
vector<int> ans;
vector<int> cnt;
int sum = 0;

void Mo::add(int idx) {
    cnt[a[idx]]++;
    if(cnt[a[idx]] % 2 == 0) ++sum;
}

void Mo::del(int idx) {
    cnt[a[idx]]--;
    if(cnt[a[idx]] % 2 == 1) --sum;
}

void solve() {
    cin >> n;
    a.resize(n);
    cnt.resize(n + 1, 0);
    FOR(n) cin >> a[i];
    cin >> q;
    ans.resize(q, 0);
 
    Mo mo(n);
 
    FOR(q) {
        int a, b;
        cin >> a >> b;
        mo.insert(a - 1, b);
    }
    mo.build();
 
    FOR(q) {
        int idx = mo.process();
        ans[idx] = sum;
    }
 
    FOR(q) {
        cout << ans[i] << endl;
    }
}
























void solve() {
    int n, q;
    cin >> n >> q;
    vi c(n);
    FOR(n) cin >> c[i], --c[i];
    vi l(q), r(q);
    FOR(q) {
        cin >> l[i] >> r[i];
        --l[i];
    }

    int w = (int)sqrt(q), nl = 0, nr = 0;
    vi order(q);
    iota(all(order), 0);
    sort(all(order), [&](int a, int b) {
        if(l[a] / w != l[b] / w) return l[a] < l[b];
        if(l[a] / w % 2 == 1) return r[a] > r[b];
        return r[a] < r[b];
    });

    vi ans(q);
    vi cnt(n, 0);
    int now = 0;

    auto add = [&](int i) {
        if(cnt[c[i]] == 0) now++;
        cnt[c[i]]++;
    };
    auto del = [&](int i) {
        if(cnt[c[i]] == 1) now--;
        cnt[c[i]]--;
    };

    FOR(q) {
        int id = order[i];
        while(nl > l[id]) add(--nl);
        while(nr < r[id]) add(nr++);
        while(nl < l[id]) del(nl++);
        while(nr > r[id]) del(--nr);
        ans[id] = now;
    }

    for(auto e : ans) cout << e << endl;
}