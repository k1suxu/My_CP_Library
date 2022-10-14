template<typename T>
struct Edge {
    int to;
    T cost;

    Edge() = default;
    Edge(int to, T cost) : to(to), cost(cost) {}

    bool operator<(const Edge<T> other) const {
        return cost < other.cost;
    }
    bool operator>(const Edge<T> other) const {
        return cost > other.cost;
    }
};

template<typename T>
vector<T> dijkstra(vector<vector<Edge<T>>> &graph, int start, T T_INF, T T_ZERO, function<T(T, T)> add) {
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
            if(dist[e.to] > add(dist[v], e.cost)) {
                dist[e.to] = add(dist[v], e.cost);
                pq.emplace(dist[e.to], e.to);
            }
        }
    }
    return dist;
}

template<typename T>
int furthest_node(vector<vector<Edge<T>>> g, int st, T T_INF, T T_ZERO, function<T(T, T)> add) {
    vector<T> dist = dijkstra(g, st, T_INF, T_ZERO, add);

    return (int)(max_element(dist.begin(), dist.end()) - dist.begin());
}

template<typename T>
vector<T> Height_of_Tree(vector<vector<Edge<T>>> g, T T_INF, T T_ZERO, function<T(T, T)> add) {
    int n = (int)g.size();

    int p1 = furthest_node(g, 0, T_INF, T_ZERO, add);
    int p2 = furthest_node(g, p1, T_INF, T_ZERO, add);

    vector<T> d1 = dijkstra<T>(g, p1, T_INF, T_ZERO, add);
    vector<T> d2 = dijkstra<T>(g, p2, T_INF, T_ZERO, add);

    vector<T> ret(n);
    for(int i = 0; i < n; i++) ret[i] = max(d1[i], d2[i]);

    return ret;
}

template<typename T>
using Graph = vector<vector<Edge<T>>>;