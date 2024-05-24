template<typename T>
struct Primitive_Edge {
    int from, to;
    T cap;
    int from_id, to_id;

    Primitive_Edge() = default;
    Primitive_Edge(int from_, int to_, T cap_, int from_id_ = -1, int to_id_ = -1) :
        from(from_), to(to_), cap(cap_), from_id(from_id_), to_id(to_id_) {}
};

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
    using pe = Primitive_Edge<T>;
    const T e = numeric_limits<T>::max();
    const T zero = 0;

    int n;
    vector<pe> primitive_edges;
    vector<vector<Edge<T>>> g, prev_graph;
    vector<int> level, iter;

    Dinic(int n) : n(n), g(n) {}

    void add_edge(int from, int to, T cap) {
        Edge<T> f(to, cap, g[to].size());
        Edge<T> t(from, zero, g[from].size());

        g[from].push_back(f);
        g[to].push_back(t);

        primitive_edges.push_back({from, to, cap, (int)g[from].size() - 1, (int)g[to].size() - 1});
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

    T flow(int s, int t, bool recover = false) {
        prev_graph = g;
        T f = zero;

        while(bfs(s, t)) {
            iter.assign(n, 0);
            T d;

            while((d = dfs(s, t, e)) > zero) f += d;
        }

        if(recover) g = prev_graph;

        return f;
    }

    void recover() {
        g = prev_graph;
    }

    //0-indexed;
    void update_cap(int i, T new_cap) {
        pe &e = primitive_edges[i];
        e.cap = new_cap;
        g[e.from][e.from_id] = {e.to, e.cap, e.to_id};
        g[e.to][e.to_id] = {e.from, 0, e.from_id};
    }
    void add_cap(int i, T add_cap) {
        primitive_edges[i].cap += add_cap;
        update_cap(i, primitive_edges[i].cap);
    }
    pe get_ith_edge(int i) {
        return primitive_edges[i];
    }
    vector<pe> get_all_edges() {
        return primitive_edges;
    }
};