// 重みなしグラフの直径
int diameter_of_tree(vector<vector<int>> g) {
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



//Weighted Graphの直径の計算
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
    // max(dist[i], dist1[i])で各点からの最遠点も求められる
}
pair<vector<long long>, vector<int>> dijkstra_restoration(vector<vector<pair<int, long long>>> &graph, int start) {
    int n = (int)graph.size();
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    vector<long long> dist(n, DIJKSTRA_INF);
    vector<int> pre(n, -1);
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
                pre[e.first] = v;
                pq.emplace(dist[e.first], e.first);
            }
        }
    }
    return make_pair(dist, pre);
    // max(dist[i], dist1[i])で各点からの最遠点も求められる
}
// 重み付きグラフの直径
// get pair of verticies the length between which is equal to the diameter of the tree
// total cost, path
pair<long long, vector<int>> diameter_of_tree_get_path(vector<vector<pair<int, long long>>> g) {
    vector<long long> dist1 = dijkstra(g, 0);
    int u = (int)(max_element(dist1.begin(), dist1.end()) - dist1.begin());
    vector<long long> dist2;
    vector<int>pre;
    tie(dist2, pre) = dijkstra_restoration(g, u);
    int v = (int)(max_element(dist2.begin(), dist2.end()) - dist2.begin());

    vector<int> path;

    while(pre[v] != -1) {
        path.push_back(v);
        v = pre[v];
    }
    path.push_back(v);
    return make_pair(*max_element(dist2.begin(), dist2.end()), path);
    // max(dist[i], dist1[i])で各点からの最遠点も求められる
}