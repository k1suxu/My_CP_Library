struct Euler_Tour {
    int n;
    vector<vector<int>> g;

    Euler_Tour(int n) : n(n), g(n) {}

    void add_edge(int a, int b, bool directed = false) {
        g[a].push_back(b);
        if(!directed) g[b].push_back(a);
    }

    vector<int> order;
    vector<int> in;
    vector<int> out;
    vector<bool> done;

    void dfs1(int v) {
        in[v] = (int)order.size();
        order.push_back(v);

        done[v] = true;

        for(auto e : g[v]) {
            if(!done[e]) dfs1(e);
        }
    }
    void dfs2(int v) {
        done[v] = true;

        for(auto e : g[v]) {
            if(!done[e]) dfs2(e);
        }
        out[v] = (int)order.size();
        order.push_back(v);
    }
    void dfs3(int v) {
        done[v] = true;
        in[v] = (int)order.size();
        order.push_back(v);

        for(auto e : g[v]) {
            if(!done[e]) dfs3(e);
        }

        out[v] = (int)order.size();
        order.push_back(v);
    }

    vector<int> query_in(int root) {
        order.clear();
        in.assign(n, -1);
        done.assign(n, false);

        dfs1(root);
        return in;
    }

    vector<int> query_out(int root) {
        order.clear();
        out.assign(n, -1);
        done.assign(n, false);

        dfs2(root);
        return out;
    }

    pair<vector<int>, vector<int>> query_in_out(int root) {
        order.clear();
        in.assign(n, -1);
        out.assign(n, -1);
        done.assign(n, false);

        dfs3(root);
        return make_pair(in, out);
    }

    vector<int> pull_request() {
        return order;
    }
};