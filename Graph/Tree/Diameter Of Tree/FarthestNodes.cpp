vector<int> farthest(vector<vector<int>> g) {
    int n = (int)g.size();

    function<vector<int>(int)> bfs = [&](int st) -> vector<int> {
        vector<int> dist(n, -1);
        dist[st] = 0;
        queue<int> que;
        que.push(st);

        while(que.size()) {
            int v = que.front();
            que.pop();

            for(auto e : g[v]) {
                if(dist[e] < 0) {
                    dist[e] = dist[v] + 1;
                    que.push(e);
                }
            }
        }

        return dist;
    };

    vector<int> dist = bfs(0);
    int id1 = (int)(max_element(dist.begin(), dist.end()) - dist.begin());
    vector<int> dist1 = bfs(id1);
    int id2 = (int)(max_element(dist1.begin(), dist1.end()) - dist1.begin());
    vector<int> dist2 = bfs(id2);
    vector<int> mx(n);
    for (int i = 0; i < n; i++) mx[i] = max(dist1[i], dist2[i]);
    return mx;
}

const long long DIJKSTRA_INF = 1e18;
vector<long long> dijkstra(vector<vector<pair<int, long long>>> &graph, int start) {
    int n = (int)graph.size();
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    vector<long long> dist(n, DIJKSTRA_INF);
    dist[start] = 0;
    pq.emplace(dist[start], start);
    while(!pq.empty()) {
        pair<long long, int> p = pq.top();
        pq.pop();
        int v = p.second;
        if(dist[v] < p.first) continue;
        for(auto e : graph[v]) {
            if(dist[e.first] > dist[v] + e.second) {
                dist[e.first] = dist[v] + e.second;
                pq.emplace(dist[e.first], e.first);
            }
        }
    }
    return dist;
}
vector<int> farthest(vector<vector<pair<int, long long>>> g) {
    vector<long long> dist = dijkstra(g, 0);
    int id1 = (int)(max_element(dist.begin(), dist.end()) - dist.begin());
    vector<long long> dist1 = dijkstra(g, id1);
    int id2 = (int)(max_element(dist1.begin(), dist1.end()) - dist1.begin());
    vector<long long> dist2 = dijkstra(g, id2);
    
    vector<long long> mx((int)g.size());
    for (int i = 0; i < (int)g.size(); i++) mx[i] = max(dist1[i], dist2[i]);
    return mx;
}


// 重みが負の辺があっても大丈夫
// dp-solution
vector<long long> farthest(vector<vector<pair<int, long long>>> g) {
    int n = (int)g.size();
    vector<long long> dp(n, 0);
    auto dfs = [&](int v, int p, auto self) -> void {
        for (auto e : g[v]) if (e.first != p) {
            self(e.first, v, self);
            dp[v] = max(dp[v], dp[e.first] + e.second);
        }
    };
    dfs(0, -1, dfs);
    vector<long long> ans = dp;
    auto dfs2 = [&](int v, int p, long long upper, long long weight, auto self) -> void {
        ans[v] = max(ans[v], upper + weight);
        vector<long long> dp_data;
        for (auto e : g[v]) if (e.first != p) {
            dp_data.emplace_back(dp[e.first] + e.second);
        }
        int child_sz = (int)dp_data.size();
        vector<long long> cuml(child_sz + 1, 0), cumr(child_sz + 1, 0);
        cuml[0] = upper + weight;
        for (int i = 0; i < child_sz; i++) cuml[i+1] = max(cuml[i], dp_data[i]);
        for (int i = child_sz-1; i >= 0; i--) cumr[i] = max(cumr[i+1], dp_data[i]);

        int id = 0;
        for (auto e : g[v]) if (e.first != p) {
            self(e.first, v, max(cuml[id], cumr[id+1]), e.second, self);
            id++;
        }
    };
    dfs2(0, -1, -1, 0, dfs2);
    return ans;
}

