struct Triangle_Mo {
private:
    ll triangleorder(int l,int r) {
        ll d = 0;
        for (ll s = 1<<logn-1; s > 1; s>>=1) {
            if (l >= s) {
                d += (36*s*s)>>4;
                l -= s;
                r -= s;
            }
            else if (l+r > s<<1) {
                d += (24*s*s)>>4;
                l = l+1;
                r = (s<<1)-r;
                swap(l,r);
            }
            else if (r > s) {
                d += (12*s*s)>>4;
                l = s-l;
                r = r-s-1;
                swap(l,r);
            }
        }
        d += l+r-1;
        return d;
    }
public:
    const int logn = 20;
    vector<int> l, r;

    Triangle_Mo(int n) {}

    /* add query [left, right) */
    void add_query(int left, int right) {
        l.push_back(left);
        r.push_back(right);
    }

    template<typename Answer_Type, void (*add)(int), void (*del)(int), Answer_Type (*get_ans)()>
    vector<Answer_Type> solve() {
        const int q = (int)l.size();
        int now_l = 0, now_r = 0;
        vector<Answer_Type> ans(q);
        vector<int> idx(q), ord(q);

        for(int i = 0; i < q; ++i) ord[i] = triangleorder(l[i], r[i]);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return ord[a] < ord[b];
        });

        for(int i = 0; i < q; ++i) {
            while(now_l > l[idx[i]]) add(--now_l);
            while(now_r < r[idx[i]]) add(now_r++);
            while(now_l < l[idx[i]]) del(now_l++);
            while(now_r > r[idx[i]]) del(--now_r);
            ans[idx[i]] = get_ans();
        }

        return ans;
    }

    template<typename Answer_Type, typename T_add, typename T_del, typename T_get_ans>
    vector<Answer_Type> solve(T_add add, T_del del, T_get_ans get_ans) {
        const int q = (int)l.size();
        int now_l = 0, now_r = 0;
        vector<Answer_Type> ans(q);
        vector<int> idx(q), ord(q);

        for(int i = 0; i < q; ++i) ord[i] = triangleorder(l[i], r[i]);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return ord[a] < ord[b];
        });

        for(int i = 0; i < q; ++i) {
            while(now_l > l[idx[i]]) add(--now_l);
            while(now_r < r[idx[i]]) add(now_r++);
            while(now_l < l[idx[i]]) del(now_l++);
            while(now_r > r[idx[i]]) del(--now_r);
            ans[idx[i]] = get_ans();
        }

        return ans;
    }
};

//const int logn = 18 -- 2*10^5くらいまでの場合
//const int logn = 20 -- 10^6くらいまでの場合
//ceil(log_2(n))くらいをとればよい