vector<vector<int>> grid_dijkstra(vector<vector<int>> g, int rx, int ry) {
    int h = g.size(), w = g[0].size();
    vector<vector<int>> dist(h, vector<int>(w, INF));
    dist[rx][ry] = g[rx][ry];
    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<pair<int, pii>>> pq;
    pq.push(make_pair(dist[rx][ry], make_pair(rx, ry)));

    while(pq.size()) {
        pair<int, pair<int, int>> p = pq.top();
        pq.pop();
        int x, y;
        tie(x, y) = p.second;

        if(dist[x][y] < p.first) continue;

        rep(d, 4) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if(nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if(dist[nx][ny] > dist[x][y] + g[nx][ny]) {
                dist[nx][ny] = dist[x][y] + g[nx][ny];
                pq.push(make_pair(dist[nx][ny], make_pair(nx, ny)));
            }
        }
    }

    return dist;
}