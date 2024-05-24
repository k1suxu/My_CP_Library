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

vector<vector<int>> tree_generator_using_UF(const int n, mt19937_64& engine, const int indexed=0) {
    UnionFind UF(n+indexed);
    vector<vector<int>> g(n+indexed);
    int edge_count = 0;

    while(edge_count < n-1) {
        int u = engine()%n+indexed;
        int v = engine()%n+indexed;

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
int get_rand(int l, int r, mt19937_64& engine) {
    return engine()%(r-l)+l;
}

// return random array consisting of non-negatove integer in [l, r)
vector<int> random_array(const int len, const int l, const int r, mt19937_64& engine) {
    vector<int> x;
    for(int i = 0; i < len; ++i) {
        x.push_back(get_rand(l, r, engine));
    }
    return x;
}

// not debugged (might contain some bugs)
vector<int> random_permutation(const int len, mt19937_64& engine, int start_val = 0) {
    vector<int> x(len);
    iota(all(x), start_val);
    shuffle(x.begin(), x.end(), engine);
    return x;
}

void my_sleep(const int millisec) {
    std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
}


//generator seeding
{
    random_device rnd;
    srand((int)time(0));
    mt19937_64 rng(rnd() * rand());
}

//[TODO]: tree_generator_using_prufer_code: プリューファー列を使ったrandom tree generatorを作る。