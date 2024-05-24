//O(|F|(|V| + |E|)) O(max flow) * O(DFS)
//max flow == min cost cut
template<typename T>
struct edge {
    int to;
    T cap;
    int rev;
    
    edge() = default;
    edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
};

template<typename T>
struct Ford_Fulkerson {
    const T e = numeric_limits<T>::max();
    int V;
    vector<vector<edge<T>>> G;
    vector<bool> used;

    Ford_Fulkerson(int v) : V(v) {
        G.resize(V);
        used.resize(V, false);
    }

    void add_edge(int from, int to, T cap) {
        edge<T> f(to, cap, G[to].size());
        G[from].push_back(f);
        edge<T> t(from, 0, G[from].size() - 1);
        G[to].push_back(t);
        //G[e.to][e.rev] = 逆辺
    }

    T dfs(int v, int t, T f) {
        if(v == t) return f;
        used[v] = true;
        
        for(edge<T>& e : G[v]) {
            if(!used[e.to] && e.cap > 0) {
                T d = dfs(e.to, t, min(f, e.cap));
                if(d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }

        return 0;
    }

    //search the max flow from S to T
    T max_flow(int s, int t) {
        T flow = 0;
        while(true) {
            used.assign(V, false);
            T f = dfs(s, t, e);
            if(f == 0) return flow;
            flow += f;
        }
    }
};

//O(|F|(|V| + |E|)) O(max flow) * O(DFS)
struct edge {
    int to;
    int cap;
    int rev;
    
    edge() = default;
    edge(int to, int cap, int rev) : to(to), cap(cap), rev(rev) {}
};

struct Ford_Fulkerson {
    int V;
    vector<vector<edge>> G;
    vector<bool> used;

    Ford_Fulkerson(int v) : V(v) {
        G.resize(V);
        used.resize(V, false);
    }

    void add_edge(int from, int to, int cap) {
        edge F(to, cap, G[to].size());
        G[from].push_back(F);
        edge T(from, 0, G[from].size() - 1);
        G[to].push_back(T);
        //G[e.to][e.rev] = 逆辺
    }

    int dfs(int v, int t, int f) {
        if(v == t) return f;
        used[v] = true;
        
        for(edge& e : G[v]) {
            if(!used[e.to] && e.cap > 0) {
                int d = dfs(e.to, t, min(f, e.cap));
                if(d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }

        return 0;
    }

    //search the max flow from S to T
    int max_flow(int s, int t) {
        int flow = 0;
        while(true) {
            used.assign(V, false);
            int f = dfs(s, t, INF);
            if(f == 0) return flow;
            flow += f;
        }
    }
};