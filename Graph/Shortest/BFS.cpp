//Graph
//Shortest
//Graph_BFS
//IN
//graph, start_position
// return dist from position
// O(N+M)
vector<int> bfs(vector<vector<int>> &g, int s) {
    int n = g.size();
    queue<int> que;
    vi dist(n, -1);
    que.push(s);
    dist[s] = 0;
    while(!que.empty()) {
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
}

vector<int> bfs(vector<vector<pair<int, int>>> &g, int s, int gl) {
    int n = g.size();
    queue<int> que;
    vi dist(n, -1);
    //first:??????_, second:?g????
    vpii nxt(n, make_pair(-1, -1));
    //first:?O????_, second:?g??????
    vpii pre(n, make_pair(-1, -1));
    que.push(s);
    dist[s] = 0;
    while(!que.empty()) {
        int v = que.front();
        que.pop();
        for(auto e : g[v]) {
            if(dist[e.first] < 0) {
                dist[e.first] = dist[v] + 1;
                nxt[v] = make_pair(e.first, e.second);
                pre[e.first] = make_pair(v, e.second);
                que.push(e.first);
            }
        }
    }

    vi used;
    
    //??
    while(pre[gl].first != -1) {
        used.push_back(pre[gl].second);
        gl = pre[gl].first;
    }
    //???_
    // used.push_back(gl);
    // while(pre[gl].first != -1) {
    //     used.push_back(pre[gl].first);
    //     gl = pre[gl].first;
    // }

    reverse(all(used));

    return used;
}