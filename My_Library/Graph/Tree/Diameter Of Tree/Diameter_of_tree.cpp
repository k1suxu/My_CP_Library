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
pair<int, int> radius_of_tree(vector<vector<int>> g) {
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
    return make_pair(id, (int)(max_element(all(dist2)) - dist2.begin()));
}