struct Random {
private:
    struct UnionFind {
        vector<int> r;
        
        UnionFind(int n) {
            r = vector<int>(n, -1);
        }

        int root(int x) {
            if(r[x] < 0) return x;
            return r[x] = root(r[x]);
        }

        bool unite(int x, int y) {
            x = root(x);
            y = root(y);
            if(x == y) return false;
            if(r[x] > r[y]) swap(x, y);
            r[x] += r[y];
            r[y] = x;
            return true;
        }

        bool issame(int x, int y) {
            return root(x) == root(y);
        }
    };

public:
    mt19937_64 rng;
    Random() {
        random_device rnd;
        srand((int)time(0));
        rng = mt19937_64(rnd() * rand());
    }

    vector<vector<int>> tree_generator_using_UF(const int n, const int indexed=0) {
        UnionFind UF(n+indexed);
        vector<vector<int>> g(n+indexed);
        int edge_count = 0;

        while(edge_count < n-1) {
            int u = rng()%n+indexed;
            int v = rng()%n+indexed;

            if(!UF.issame(u, v)) {
                g[u].push_back(v);
                g[v].push_back(u);
                UF.unite(u, v);
                ++edge_count;
            }
        }
        return g;
    }

    vector<vector<int>> tree_generator_using_prufer_code() {}

    // return random non-negative integer in [l, r)
    int get_int(int l, int r) {
        return rng()%(r-l)+l;
    }

    // return random array consisting of non-negatove integer in [l, r)
    vector<int> get_array(const int len, const int l, const int r) {
        vector<int> x;
        for(int i = 0; i < len; ++i) {
            x.push_back(get_int(l, r));
        }
        return x;
    }

    // not debugged (might contain some bugs)
    vector<int> get_permutation(const int len, int start_val = 0) {
        vector<int> x(len);
        iota(all(x), start_val);
        shuffle(x.begin(), x.end(), rng);
        return x;
    }

    void my_sleep(const int millisec) {
        std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
    }
};