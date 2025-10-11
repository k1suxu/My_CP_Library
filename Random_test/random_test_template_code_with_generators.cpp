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
        vpii edges;
        int edge_count = 0;

        while(edge_count < n-1) {
            int u = rng()%n+indexed;
            int v = rng()%n+indexed;

            if(!UF.issame(u, v)) {
                g[u].push_back(v);
                g[v].push_back(u);
                UF.unite(u, v);
                ++edge_count;
                edges.emplace_back(u, v);
            }
        }
        // edgesを返せば辺を返せる
        return g;
    }

    vector<vector<int>> tree_generator_using_prufer_code() {}

    // return random non-negative integer in [l, r)
    int get_int(int l, int r) {
        return rng()%(r-l)+l;
    }
    int get_ll(ll l, ll r) {
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
    vector<ll> get_array_ll(const int len, const ll l, const ll r) {
        vector<ll> x;
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

    string get_string(const int len, bool lower, bool upper, bool number) {
        vector<char> char_set;
        if(lower) for(int i = 0; i < 26; i++) char_set.push_back((char)('a' + i));
        if(upper) for(int i = 0; i < 26; i++) char_set.push_back((char)('A' + i));
        if(number) for(int i = 0; i < 9; i++) char_set.push_back((char)('0' + i));
        return get_string(len, char_set);
    }

    string get_string(const int len, vector<char> char_set) {
        string s;
        for(int i = 0; i < len; i++) s.push_back(char_set[get_int(0, (int)char_set.size())]);
        return s;
    }

    void my_sleep(const int millisec) {
        std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
    }
} rd;