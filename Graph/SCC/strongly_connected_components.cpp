struct StronglyConnectedComponents {
    int n;
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
        for(auto e : order) if(component[e] == -1) dfs2(e, k), k++;
    }

    //return if vertex(u, v) in same strongly connected component
    bool issame(int u, int v) const {
        return component[u] == component[v];
    }

    int number_of_components() const {
        return *max_element(component.begin(), component.end()) + 1;
    }

    vector<vector<int>> get_compressed_DAG() {
        //conponentごとにまとめたグラフ
        const int G_SZ = *max_element(component.begin(), component.end()) + 1;
        vector<vector<int>> rebuilded_graph(G_SZ);
        for(int v = 0; v < n; v++) {
            for(auto e : graph[v]) {
                if(component[v] != component[e]) {
                    rebuilded_graph[component[v]].emplace_back(component[e]);
                }
            }
        }
        for(auto &e : rebuilded_graph) {
            sort(e.begin(), e.end());
            e.erase(unique(e.begin(), e.end()), e.end());
        }
        return rebuilded_graph;
    }
    
    vector<vector<int>> pull_groups() {
        const int G_SZ = *max_element(component.begin(), component.end()) + 1;
        vector<vector<int>> groups(G_SZ);
        
        for(int i = 0; i < n; i++) {
            groups[component[i]].push_back(i);
        }

        return groups;
    }
};

using SCC = StronglyConnectedComponents;
// SCCのgroupsはグループ一つ一つを頂点としたときにトポロジカルソート順で入っている。
// cf: https://judge.yosupo.jp/problem/scc