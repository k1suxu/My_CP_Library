template<typename T>
struct Edge {
    int to;
    T cap;
    int rev;

    Edge() = default;
    Edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
};

template<typename T>
struct Bipartite_Matching {
    const T E = numeric_limits<T>::max();
    const T zero = 0;
    int n;
    vector<vector<Edge<T>>> g;
    vector<int> level, iter;

    Bipartite_Matching(int n) : n(n), g(n) {}

    void add_edge(int from, int to, T cap) {
        Edge<T> f(to, cap, g[to].size());
        Edge<T> t(from, zero, g[from].size());
        g[from].push_back(f);
        g[to].push_back(t);
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

    T dfs(int v, int t, T flow) {
        if(v == t) return flow;

        for(int& i = iter[v]; i < (int)g[v].size(); i++) {
            Edge<T>& e = g[v][i];

            if(e.cap > 0 && level[v] < level[e.to]) {
                T d = dfs(e.to, t, min(flow, e.cap));

                if(d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }

        return 0;
    }

    
    T max_matcing(int s, int t) {
        T f = zero;

        while(bfs(s, t)) {
            iter.assign(n, 0);
            T d;
            while((d = dfs(s, t, E)) > zero) f += d;
        }

        return f;
    }
};

//O(|V||E|)