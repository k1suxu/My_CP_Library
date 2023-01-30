//Graph
//SCC
//strongly_connected_components
//
//data structure

struct StronglyConnectedComponents {
    int n;
    //reversed_graph
    //辺を逆向きに貼りなおしたグラフ。
    vector<vector<int>> graph, reversed_graph;
    vector<int> order, component;
    vector<bool> used;
    void dfs(int v) {
        used[v] = 1;
        for(auto e : graph[v]) {
            if(used[e]) continue;
            dfs(e);
        }
        order.push_back(v);
    }
    void dfs2(int v, int k) {
        component[v] = k;
        for(auto e : reversed_graph[v]) {
            if(component[e] == -1) dfs2(e, k);
        }
    }

    StronglyConnectedComponents(vector<vector<int>> &G_) {
        n = G_.size();
        graph = G_;
        reversed_graph.resize(n);
        component.assign(n, -1);
        used.resize(n);
        for(int v = 0; v < n; v++) {
            for(auto e : graph[v]) reversed_graph[e].push_back(v);
        }
        for(int v = 0; v < n; v++) if(!used[v]) dfs(v);
        int k = 0;
        //topoogical sort in all the sub trees
        reverse(order.begin(), order.end());
        //いままでの強連結成分に含まれていなかったらDFS
        for(auto e : order) if(component[e] == -1) dfs2(e, k), k++;
    }

    //return if vertex(u, v) in same strongly connected component
    bool issame(int u, int v) {
        return component[u] == component[v];
    }

    vector<vector<int>> rebuild() {
        //コンポーネント数
        int N = *max_element(component.begin(), component.end()) + 1;
        vector<vector<int>> rebuilded_graph(N);
        set<pair<int, int>> connected;
        for(int v = 0; v < N; v++) {
            for(auto e : graph[v]) {
                //同じ強連結成分に含まれていなくてかつ
                //その二つの強連結成分がグラフ上でつながれていなかったら
                if(component[v] != component[e] && !connected.count(make_pair(v, e))) {
                    connected.insert(make_pair(v, e));
                    rebuilded_graph[component[v]].push_back(component[e]);
                }
            }
        }
        return rebuilded_graph;
    }
};