template<typename T_Weight>
struct Weighted_UnionFind {
    vector<int> r;
    vector<T_Weight> diff_weight;

    Weighted_UnionFind(int n) {
        r = vector<int>(n, -1);
        diff_weight = vector<T_Weight>(n, 0);
    }

    int root(int x) {
        if(r[x] < 0) return x;
        int R = root(r[x]);
        diff_weight[x] += diff_weight[r[x]];
        return r[x] = R;
    }
    //weight[y] - weight[x] = w;
    void unite(int x, int y, T_Weight w) {
        w += weight(x);
        w -= weight(y);
        x = root(x);
        y = root(y);
        if(x == y) return;
        
        if(r[x] < r[y]) swap(x, y), w = -w;
        r[y] = x;
        diff_weight[y] = w;
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    T_Weight weight(int x) {
        root(x);
        return diff_weight[x];
    }
    //return diff(x, y) if x, y stand in the same tree
    //weight[y] - weight[x]
    T_Weight diff(int x, int y) {
        if(!issame(x, y)) return INF;
        return weight(y) - weight(x);
    }

    // 最も軽いやつが先頭
    // {{vertex_id, its weight}}
    // 一番軽いやつは常に重みが0になるようにしている(基準値は常に0)
    vector<vector<pair<int, T_Weight>>> decompose() {
        vector<tuple<int, int, T_Weight>> p;
        p.reserve((int)r.size());
        for(int i = 0; i < (int)r.size(); i++) p.emplace_back(make_tuple(root(i), i, weight(i)));
        sort(all(p), [&](const tuple<int, int, T_Weight> &x, const tuple<int, int, T_Weight> &y) -> bool {
            if(get<0>(x) != get<0>(y)) return get<0>(x) < get<0>(y);
            return get<2>(x) < get<2>(y);
        });

        vector<vector<pair<int, T_Weight>>> ret;
        int pre = -1;
        for(const auto  &e : p) {
            if(pre != get<0>(e)) ret.emplace_back();
            ret.back().emplace_back(get<1>(e), get<2>(e));
            pre = get<0>(e);
        }

        for(vector<pair<int, T_Weight>> &e : ret) {
            T_Weight minus = e[0].second;
            for(pair<int, T_Weight> &f : e) f.second -= minus;
        }

        return ret;
    }
};

//[TODO]: 矛盾を返すようにしたい