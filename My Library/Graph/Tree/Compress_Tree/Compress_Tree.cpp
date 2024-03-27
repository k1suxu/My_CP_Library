struct Compress_Tree {
    vector<int> fs, ls;
    vector<int> depth;
    vector<vector<int>> st;
    vector<int> lg;
    
    int cur;
    vector<vector<int>> g;

    vector<int> stk;

    Compress_Tree() = default;
    Compress_Tree(int n) {
        int nlog = 1;
        int k = 1;
        while(k <= n) {
            k *= 2;
            ++nlog;
        }
        fs.resize(n);
        ls.resize(n);
        depth.resize(n);
        st.resize(nlog, vector<int>(3*n));
        lg.resize(3*n);
        g.resize(n);
        stk.resize(2*n);
    }

    void add_edge(int u, int v) {
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    
    void ett_dfs(int v, int p, int d) {
        st[0][fs[v] = cur++] = v;
        depth[v] = d;
        for(int w : g[v]) {
            if(w == p) continue;
            ett_dfs(w, v, d+1);
            st[0][cur++] = v;
        }
        ls[v] = cur-1;
    }
    
    void build() {
        cur = 0;
        // Euler tour technique
        ett_dfs(0, -1, 0);
        lg[0] = lg[1] = 0;
        for(int i=2; i<=cur; ++i) lg[i] = lg[i >> 1] + 1;
        
        // Sparse Table
        for(int i=0, b=1; i<lg[cur]; ++i, b<<=1) {
            for(int j=0; j<(cur - (b<<1) + 1); ++j) {
                st[i+1][j] = (depth[st[i][j]] <= depth[st[i][j+b]] ? st[i][j] : st[i][j+b]);
            }
        }
    }
    
    inline int lca(int u, int v) {
        int x = fs[u], y = fs[v];
        if(x > y) swap(x, y);
        int l = lg[y - x + 1];
        return (depth[st[l][x]] <= depth[st[l][y - (1 << l) + 1]] ? st[l][x] : st[l][y - (1 << l) + 1]);
    }

    // 頂点vsを含むAuxiliary Treeを構築する
    // 結果はg0に入る
    // 返り値はAuxiliary Treeの根頂点
    int get_compressed_tree(vector<int> &vs, vector<vector<int>>& g0) {
        assert(g0.size() == g.size());
        sort(vs.begin(), vs.end(), [&](int x, int y) -> bool { return fs[x] < fs[y]; });
        int k = vs.size();
        for(int i=0; i<k-1; ++i) {
            vs.push_back(lca(vs[i], vs[i+1]));
        }
        sort(vs.begin(), vs.end(), [&](int x, int y) -> bool { return fs[x] < fs[y]; });
        int prv = -1;
        int cur = 0;
        for(int i=0; i<vs.size(); ++i) {
            int v = vs[i];
            if(prv == v) continue;
            while(cur > 0 && ls[stk[cur-1]] < fs[v]) --cur;
            if(cur > 0) {
                g0[stk[cur-1]].push_back(v);
            }
            g0[v].clear();
            stk[cur++] = v;
            prv = v;
        }
        return stk[0];
    }
};