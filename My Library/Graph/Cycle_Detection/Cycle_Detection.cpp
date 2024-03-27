// cf: https://judge.yosupo.jp/problem/cycle_detection_undirected
// Graph: edge:first->行き先, second->辺番号
// 戻り値: first->頂点列, second->辺列
// 単純グラフじゃなくてもOK
// O(N)
pair<vector<int>, vector<int>> cycle_detect(const vector<vector<pair<int, int>>> &g) {
    const int n = (int)g.size();
    bool flag = false;
    pair<vector<int>, vector<int>> ans;
    vector<pair<int, int>> nxt(n);
    vector<bool> done(n, false), cur_path_done(n, false);

    auto dfs = [&](int v, auto self) -> void {
        if(flag) return;
        done[v] = true;
        cur_path_done[v] = true;

        for(auto e : g[v]) {
            if(flag) return;

            if(!done[e.first]) {
                nxt[v] = e;
                self(e.first, self);
            }else if(cur_path_done[e.first] && (nxt[e.first].second != e.second || e.first == v)) {
                flag = true;
                int cur = e.first;
                ans.first.push_back(cur);
                while(cur != v) {
                    ans.second.push_back(nxt[cur].second);
                    cur = nxt[cur].first;
                    ans.first.push_back(cur);
                }
                ans.second.push_back(e.second);
            }
        }

        cur_path_done[v] = false;
    };

    for(int i = 0; i < n; i++) if(!done[i]) {
        dfs(i, dfs);
        if(flag) return ans;
    }
    
    return {{}, {}};
}