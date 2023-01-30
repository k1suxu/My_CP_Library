struct Is_Bipartite {
    int n;
    vector<int> color;
    vector<vector<int>> g;

    Is_Bipartite(int n) : n(n), g(n) {}
    Is_Bipartite(vector<vector<int>> g) : n(g.size()), g(g) {}

    void add_edge(int a, int b) {
        g[a].push_back(b);
        g[b].push_back(a);
    }

    bool dfs(int v, int col) {
        color[v] = col;

        for(auto e : g[v]) {
            if(color[e] == col) {
                return false;
            }
            if(color[e] == -1) {
                if(!dfs(e, 1-col)) return false;
            }
        }
        
        return true;
    }

    bool generate() {
        color.assign(n, -1);
        bool flag = true;
        for(int i = 0; i < n; i++) {
            if(color[i] == -1) {
                bool now = dfs(i, 0);
                flag = (flag && now);
            }
        }
        return flag;
    }
};