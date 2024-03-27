// Graph
// Shortest
// given Graph(weighted, strict direction), and Startnode
// give back All the shortest Path from Startnode
// O(V log E + E log E)
// O((V+E) log E)
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

pair<vector<ll>, vector<int>> dijkstra_with_pre_calc(vector<vector<pair<int, ll>>> &graph, int start) {
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

// s->t最短パスを返す(頂点)
// first=-1 -> パスが存在しない
template<typename T>
pair<T, vector<int>> dijkstra_restoration(const vector<vector<pair<int, T>>> &g, const int st, const int gl) {
    const int n = (int)g.size();
    vector<T> dist(n, numeric_limits<T>::max());
    vector<int> pre(n, -1);
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
    dist[st] = 0;
    pq.emplace(dist[st], st);

    while(!pq.empty()) {
        pair<T, int> p = pq.top();
        pq.pop();
        int v = p.second;

        if(dist[v] < p.first) continue;

        for(auto e : g[v]) if(dist[e.first] > dist[v] + e.second) {
            dist[e.first] = dist[v] + e.second;
            pq.emplace(dist[e.first], e.first);
            pre[e.first] = v;
        }
    }

    if(pre[gl] == -1) return {{-1}, {}};

    vector<int> path = {gl};
    int cur = gl;
    while(pre[cur] != -1) {
        cur = pre[cur];
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());

    return make_pair(dist[gl], path);
}

template<typename T>
vector<T> dijkstra(vector<vector<pair<int, T>>> &graph, int start, T T_INF, T T_ZERO, function<T(T, T)> add) {
    int n = (int)graph.size();
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;

    vector<T> dist(n, T_INF);
    dist[start] = T_ZERO;
    pq.emplace(dist[start], start);

    while(!pq.empty()) {
        pair<T, int> p = pq.top();
        pq.pop();
        int v = p.second;
        if(dist[v] < p.first) continue;
        for(auto e : graph[v]) {
            if(dist[e.first] > add(dist[v], e.second)) {
                dist[e.first] = add(dist[v], e.second);
                pq.emplace(dist[e.first], e.first);
            }
        }
    }
    return dist;
}

