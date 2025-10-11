vector<vector<int>> tree_hash(vector<vector<int>> tree, int root, int NUM_HASH) {
    const int n = (int)tree.size();
    vector<int> mods(NUM_HASH);
    vector<vector<int>> depth_randoms;
    vector<vector<int>> hash(n, vector<int>(NUM_HASH, 1));

    random_device rnd;
    srand((int)time(0));
    mt19937 rng(rnd() * rand());
    for (int i = 0; i < NUM_HASH; i++) {
        mods[i] = (1<<30) + rng() % (1<<30);
    }

    function<void(int, int, int)> dfs = [&](int v, int p, int d) -> void {
        if ((int)depth_randoms.size() <= d) {
            vector<int> randoms(NUM_HASH);
            for (int i = 0; i < NUM_HASH; i++) randoms[i] = rng() % mods[i];
            depth_randoms.emplace_back(randoms);
        }
        for (int &child : tree[v]) if (child != p) {
            dfs(child, v, d + 1);
            for (int i = 0; i < NUM_HASH; i++) {
                hash[v][i] = (hash[v][i] * ((depth_randoms[d][i] + hash[child][i]) % mods[i])) % mods[i];
            }
        }
    };
    dfs(root, -1, 0);

    return hash;
}

// verifyが弱いかも