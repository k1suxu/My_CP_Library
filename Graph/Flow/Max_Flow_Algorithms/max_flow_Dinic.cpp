template<typename T>
struct Edge {
    int to;
    T cap;
    int rev;

    Edge() = default;
    Edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
};

template<typename T>
struct Dinic {
    const T e = numeric_limits<T>::max();
    const T zero = 0;

    int n;
    vector<vector<Edge<T>>> g;
    vector<int> level, iter;
    vector<pair<int, int>> Edge_ID;

    Dinic(int n) : n(n), g(n) {}

    int add_edge(int from, int to, T cap) {
        Edge<T> f(to, cap, g[to].size());
        Edge<T> t(from, zero, g[from].size());

        int Now_Edge_ID = (int)Edge_ID.size();
        Edge_ID.emplace_back(from, (int)g[from].size());

        g[from].push_back(f);
        g[to].push_back(t);
        
        return Now_Edge_ID;
    }

    bool bfs(int s, int t) {
        level.assign(n, -1);
        level[s] = 0;
        queue<int> que;
        que.push(s);

        while(que.size()) {
            int v = que.front();
            que.pop();

            for(auto e : g[v]) {
                if(e.cap > zero && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    T dfs(int v, int t, T f) {
        if(v == t) return f;

        for(int& i = iter[v]; i < g[v].size(); i++) {
            Edge<T>& e = g[v][i];

            if(e.cap > zero && level[v] < level[e.to]) {
                T d = dfs(e.to, t, min(f, e.cap));

                if(d > zero) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }

        return zero;
    }

    T flow(int s, int t) {
        T f = zero;

        while(bfs(s, t)) {
            iter.assign(n, 0);
            T d;

            while((d = dfs(s, t, e)) > zero) f += d;
        }

        return f;
    }

    //sから見たときのmin_cutにおいてs側に所属するかどうかを全頂点に対して返すO(N)
    pair<T, vector<bool>> min_cut(int s, int t) {
        T max_flow = flow(s, t);
        vector<bool> visited(n, false);
        queue<int> que;
        visited[s] = true;
        que.push(s);
        while(!que.empty()) {
            int v = que.front();
            que.pop();
            for(auto e : g[v]) if(e.cap != 0 && !visited[e.to]) {
                visited[e.to] = true;
                que.push(e.to);
            }
        }
        return make_pair(max_flow, visited);
    }

    Edge<T> get_edge(int edge_id) {
        return g[Edge_ID[edge_id].first][Edge_ID[edge_id].second];
    }
};

//bipartite matching 
//TODO: make faster Dinic using dynamic tree structure and current edge structure
//燃やす埋めるref: https://qiita.com/ningenMe/items/69ed7ce43c9cd0a2de38

//add_edgeにintの返り値を持たせて、id = add_edge(), get_edge(id) = 元の辺　みたいにしたい。
//min_cut : s側ならばtrueのvector<bool>を返す。
//cut[from] = true && cut[to] = false --- from->toの辺が最小カットで切られる辺の一つ