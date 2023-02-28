//–³ŒÀ‰ñˆÚ“®‚Å‚«‚¿‚á‚¤‚â‚Â‚ÌŒÂ”
struct in_cycle {
    int n;
    vector<vector<int>> g;
    vector<int> deg;

    in_cycle() = default;
    in_cycle(int N) : n(N), g(N), deg(N) {}
    in_cycle(int N, int M, int minus = 1) : n(N), g(N), deg(N) {
        read(M, minus);
    }

    void add_edge(int from, int to, int minus = 1) {
        from = from - minus;
        to = to - minus;
        g[to].push_back(from);
        deg[from]++;
    }

    void read(int m, int minus = 1) {
        for(int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            add_edge(u, v, minus);
        }
    }

    vector<bool> To_or_In_Cycle() {
        vector<bool> cycle(n, true);
        queue<int> que;
        for(int i = 0; i < n; i++) {
            if(deg[i] == 0) {
                que.push(i);
                cycle[i] = false;
            }
        }

        while(que.size()) {
            int v = que.front();
            que.pop();

            for(auto e : g[v]) {
                deg[e]--;
                if(deg[e] == 0) {
                    que.push(e);
                    cycle[e] = false;
                }
            }
        }

        return cycle;
    }
};