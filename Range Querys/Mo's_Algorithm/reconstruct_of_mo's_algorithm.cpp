struct Mo {
    vector<int> left, right, order;
    int width;
    int nl, nr, ptr;

    // n: 必要区間の長さ(クエリ数ではない)
    Mo(int n) : width((int) sqrt(n)), nl(0), nr(0), ptr(0) {}

    /* [l, r) */ 
    void insert(int l, int r) {
        left.push_back(l);
        right.push_back(r);
    }

    void build() {
        order.resize(left.size());
        iota(begin(order), end(order), 0);
        sort(begin(order), end(order), [&](int a, int b) {
            if(left[a] / width != left[b] / width) return left[a] < left[b];
            if(left[a] / width % 2 == 1) return right[a] > right[b];
            return right[a] < right[b];
        });
    }

    template<typename FA, typename FD>
    int process(const FA &add, const FD &del) {
        if(ptr == order.size()) return (-1);
        const auto id = order[ptr];
        while(nl > left[id]) add(--nl);
        while(nr < right[id]) add(nr++);
        while(nl < left[id]) del(nl++);
        while(nr > right[id]) del(--nr);
        return (order[ptr++]);
    }

    template<typename FA, typename FD, typename FG>
    void solve(const FA &add, const FD &del, const FG &get_ans) {
        int q = (int)left.size();
        build();
        for(int i = 0; i < q; ++i) {
            int idx = process(add, del);
            get_ans(idx);
        }
    }
};