int radius_of_tree(vector<vector<int>> g) {
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

    vector<int> dist1 = bfs(0);
    int id = (int)(max_element(all(dist1)) - dist1.begin());
    vector<int> dist = bfs(id);
    return *max_element(all(dist));
}

//get pair of verticies the length between which is equal to the diameter of the tree
//total cost, path
vector<int> dijkstra(vector<vector<pair<int, int>>> &graph, int start) {
    int n = (int)graph.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(n, INF);
    dist[start] = 0;
    pq.emplace(dist[start], start);
    while(!pq.empty()) {
        pair<int, int> p = pq.top();
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
pair<vector<ll>, vector<int>> dijkstra_restoration(vector<vector<pair<int, ll>>> &graph, int start) {
    int n = (int)graph.size();
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    vector<ll> dist(n, INF);
    vector<int> pre(n, -1);
    dist[start] = 0;
    pq.emplace(dist[start], start);
    while(!pq.empty()) {
        pair<ll, int> p = pq.top();
        pq.pop();
        int v = p.second;
        if(dist[v] < p.first) continue;
        for(auto e : graph[v]) {
            if(dist[e.first] > dist[v] + e.second) {
                dist[e.first] = dist[v] + e.second;
                pre[e.first] = v;
                pq.emplace(dist[e.first], e.first);
            }
        }
    }
    return make_pair(dist, pre);
}
pair<ll, vector<int>> radius_of_tree_get_path(vector<vector<pair<int, ll>>> g) {
    vector<ll> dist1 = dijkstra(g, 0);
    int u = (int)(max_element(dist1.begin(), dist1.end()) - dist1.begin());
    vector<ll> dist2;
    vector<int>pre;
    tie(dist2, pre) = dijkstra_restoration(g, u);
    int v = (int)(max_element(dist2.begin(), dist2.end()) - dist2.begin());

    vector<int> path;

    while(pre[v] != -1) {
        path.push_back(v);
        v = pre[v];
    }
    path.push_back(v);
    return make_pair(*max_element(all(dist2)), path);
}