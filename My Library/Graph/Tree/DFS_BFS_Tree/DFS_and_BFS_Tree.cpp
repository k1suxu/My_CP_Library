vector<vector<int>> bfs_tree(vector<vector<int>> g, int root) {
    int n = (int)g.size();
    vector<vector<int>> ret;
    vector<bool> done(n, false);
    done[root] = true;
    queue<int> que;
    que.push(root);

    while(que.size()) {
        int v = que.front();
        que.pop();

        for(auto e : g[v]) {
            if(!done[e]) {
                done[e] = true;
                que.push(e);
                ret[v].push_back(e);
                ret[e].push_back(v);
            }
        }
    }

    return ret;
}

vector<pair<int, int>> bfs_tree_edges(vector<vector<int>> g, int root) {
    int n = (int)g.size();
    vector<pair<int, int>> ret;
    vector<bool> done(n, false);
    done[root] = true;
    queue<int> que;
    que.push(root);

    while(que.size()) {
        int v = que.front();
        que.pop();

        for(auto e : g[v]) {
            if(!done[e]) {
                done[e] = true;
                que.push(e);
                ret.emplace_back(v, e);
            }
        }
    }

    return ret;
}

vector<vector<int>> dfs_tree(vector<vector<int>> g, int root) {
    int n = (int)g.size();
    vector<vector<int>> ret;
    vector<bool> done(n, false);

    auto dfs = [&](int v, auto self) -> void {
        done[v] = true;
        for(auto e : g[v]) {
            if(!done[e]) {
                ret[v].push_back(e);
                ret[e].push_back(v);
                self(e, self);
            }
        }
    };

    dfs(root, dfs);
    return ret;
}

vector<pair<int, int>> dfs_tree_edges(vector<vector<int>> g, int root) {
    int n = (int)g.size();
    vector<pair<int, int>> ret;
    vector<bool> done(n, false);

    auto dfs = [&](int v, auto self) -> void {
        done[v] = true;
        for(auto e : g[v]) {
            if(!done[e]) {
                ret.emplace_back(v, e);
                self(e, self);
            }
        }
    };

    dfs(root, dfs);
    return ret;
}