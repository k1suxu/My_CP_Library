struct LowLink {
    const vector<vector<int>> &g;
    vector<int> used ,ord, low;
    vector<int> articulation_points; //articulation points;
    vector<pair<int, int>> bridges; // bridge
    
    LowLink(const vector<vector<int>> &g_) : g(g_) {
        int si = (int)g.size();
        used.assign(si, 0);
        ord.assign(si, 0);
        low.assign(si, 0);
        int k = 0;
        for(int i = 0; i < si; i++) {
            if(!used[i]) k = dfs(i, k, -1);
        }
        //sort(articulation_points.begin(), articulation_points.end());
        //sort(bridges.begin(), bridges.end());
    }

    // id:探索中の頂点
    // k:dfsで何番目に探索するか
    // parent:idの親
    int dfs(int id, int k, int parent) {
        used[id] = true;
        ord[id] = k++;
        low[id] = ord[id];
        bool is_aps = false;
        int count = 0; // number of sons
        for(auto &e : g[id]) {
            if(!used[e]) {
                count++;
                k = dfs(e, k, id);
                low[id] = min(low[id], low[e]);
                if(parent != -1 && ord[id] <= low[e]) is_aps = true;
                if(ord[id] < low[e]) bridges.emplace_back(min(id, e), max(id, e));
            } else if(e != parent) {
                low[id] = min(low[id], ord[e]);
            }
        }
        if(parent == -1 && count >= 2) is_aps = true;
        if(is_aps) articulation_points.push_back(id);
        return k;
    }
};



struct LowLink {
    const vector<vector<int>> &g;
    vector<int> used ,ord, low;
    vector<int> articulation_points; //articulation points;
    vector<pair<int, int>> bridges; // bridge

    //low[u] -- uからの後退辺を高々1回まで用いて到達できる頂点wについてord[w]の最小値
    LowLink(const vector<vector<int>> &g_) : g(g_) {
        int si = (int)g.size();
        used.assign(si, 0);
        ord.assign(si, 0);
        low.assign(si, 0);
        int k = 0;
        for(int i = 0; i < si; i++) {
            if(!used[i]) k = dfs(i, k, -1);
        }
        //sort(articulation_points.begin(), articulation_points.end());
        //sort(bridges.begin(), bridges.end());
    }

    // id:探索中の頂点
    // k:dfsで何番目に探索するか
    // parent:idの親
    int dfs(int id, int k, int parent) {
        used[id] = true;
        ord[id] = k++;
        low[id] = ord[id];
        bool is_aps = false;
        int count = 0; // number of sons
        for(auto &e : g[id]) {
            if(!used[e]) {
                count++;
                k = dfs(e, k, id);
                low[id] = min(low[id], low[e]);
                //rootでなくてarticulation_pointである。
                /*In DFS tree(order)
                v を根とする DFS tree の部分木において、
                u の祖先への後退辺を持つ頂点が存在しない(関節点)
                u を含むvの祖先への後退辺を持つ頂点が存在しない(橋)
                //後退辺とはDFStreeにおいて使用されなかった辺のこと(DFSの戻りではないことに注意)
                          1
                         / \
                        2   5
                       / \
                      3 - 4
                    関節点-> 1, 2
                    各ノードの直接の祖先のうちordが最小の点のみが関節点
                    3の場合->2
                    4の場合->min(2, 3) -> 2
                */  
                if(parent != -1 && ord[id] <= low[e]) is_aps = true;
                if(ord[id] < low[e]) bridges.emplace_back(min(id, e), max(id, e));
            } else if(e != parent) {
                //id -> e が後退辺
                //直接戻る辺は数えない
                low[id] = min(low[id], ord[e]);
            }
        }
        if(parent == -1 && count >= 2) is_aps = true;
        if(is_aps) articulation_points.push_back(id);
        return k;
    }
};