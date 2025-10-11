//Graph
//Shortest
//Bellman_Ford
//
//declare struct (EDGE)


/*****************************************/
// 経路上に負のサイクルがあるかあるか確認する場合は
// contained_negative_cycleを使うこと!!普通のベルマンフォードでは
// 3 4
// 1 2 1  
// 2 2 1  
// 2 3 1  
// 1 3 100000  
// 以上のようなケースに対応できない!!
/*****************************************/
struct edge {
    int from;
    int to;
    int cost;

    edge() = default;
    edge(int f, int t, int c) : from(f), to(t), cost(c) {}
};

bool bellman_ford(const vector<edge> graph, int number_of_vertex, int start, vector<int> &dist) {
    dist.resize(number_of_vertex, INF);
    dist[start] = 0;
    int cnt = 0;
    while(cnt < number_of_vertex) {
        bool end = true;
        for(auto e : graph) {
            if(dist[e.from] != INF && dist[e.from] + e.cost < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.cost;
                end = false;
            }
        }
        if(end) break;
        cnt++;
    }
    return cnt != number_of_vertex;
}
// 負閉路あったらfalse返す


//最短経路上に負のサイクルがあるような頂点の集合
struct edge {
    int from, to, cost;

    edge() = default;
    edge(int f, int t, int c) : from(f), to(t), cost(c) {}
};

vector<bool> contained_negative_cycle(const vector<edge> g, int n, int st, vector<int> &dist) {
    dist.resize(n, INF);
    dist[st] = 0;
    int cnt = 0;

    vector<bool> negative(n, false);

    while(cnt < n) {
        bool end = true;

        for(auto e : g) {
            if(dist[e.from] == INF) continue;

            if(dist[e.from] + e.cost < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.cost;
                end = false;
            }
        }

        if(end) break;

        cnt++;
    }

    if(cnt != n) {
        return negative;
    }

    for(int i = 0; i < n; i++) {
        for(auto e : g) {
            if(dist[e.from] == INF) continue;

            if(dist[e.from] + e.cost < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.cost;
                negative[e.to] = true;
            }

            if(negative[e.from]) {
                negative[e.to] = true;
            }
        }

        cnt++;
    }
    
    return negative;
}



//set of a negative cycle
struct edge {
    int from;
    int to;
    int cost;

    edge() = default;
    edge(int f, int t, int c) : from(f), to(t), cost(c) {}
};

//O(N(N + M))
//return all the vertexes in a negative cycle
//一つしか見つけられない？？
//need to debug
vector<int> bellman_ford(const vector<edge> graph, int number_of_vertex, int start, vector<int> &dist, vector<int> &pre) {
    pre.resize(number_of_vertex, -1);
    dist.resize(number_of_vertex, INF);
    dist[start] = 0;
    int cnt = 0;
    while(cnt < number_of_vertex) {
        bool end = true;
        for(auto e : graph) {
            if(dist[e.from] != INF && dist[e.from] + e.cost < dist[e.to]) {
                pre[e.to] = e.from;
                dist[e.to] = dist[e.from] + e.cost;
                end = false;
            }
        }
        if(end) break;
        cnt++;
    }

    if(cnt == number_of_vertex) {
        vector<bool> done(number_of_vertex, false);

        for(int iz = 0; iz < number_of_vertex; iz++) {
            if(done[iz]) continue;

            int i = iz;

            vector<bool> now_used(number_of_vertex, false);
            vector<int> ans;

            while(pre[i] != -1) {
                if(now_used[i]) {
                    return ans;
                }
                ans.push_back(i);
                now_used[i] = true;
                done[i] = true;
                i = pre[i];
            }
        }

        return {};
    }else {
        return {};
    }
}