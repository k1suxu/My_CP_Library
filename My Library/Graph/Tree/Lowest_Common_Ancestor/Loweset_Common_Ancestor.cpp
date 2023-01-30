//LCA using doubling algorithm
template<typename T = int>
class LCA{
    private:
    T root;
    int k;
    vector<vector<T>> dp;
    vector<T> depth;

    public:
    LCA(const vector<vector<T>>& _G, const int _root = 0) {
        int n = (int)_G.size();
        root = _root;
        k = 1;
        int two = 2;
        while(two < n) {
            two <<= 1;
            k++;
        }

        dp = vector<vector<T>>(k+1, vector<T>(n, -1));
        depth.resize(n);
        function<void(int, int)> _dfs = [&](int v, int p) {
            dp[0][v] = p;
            for(auto nv : _G[v]) {
                if(nv == p) continue;
                depth[nv] = depth[v] + 1;
                _dfs(nv, v);
            }
        };
        _dfs(root, -1);

        for(int i = 0; i < k; i++) {
            for(int j = 0; j < n; j++) {
                if(dp[i][j] == -1) continue;
                dp[i + 1][j] = dp[i][dp[i][j]];
            }
        }
    }

    int get(int u, int v) {
        if(depth[u] < depth[v]) swap(u, v);
        if(depth[u] != depth[v]) {
            long long d = depth[u] - depth[v];
            for(int i = 0; i < k; i++) if((d>>i) & 1) u = dp[i][u];
        }

        if(u==v) return u;

        //一致する点のひとつ前のところに移動。
        for(int i = k; i >= 0; i--) {
            if(dp[i][u] != dp[i][v]) {
                u = dp[i][u];
                v = dp[i][v];
            }
        }
        return dp[0][u];
    }

    int get_distance(const int u, const int v) {
        int lca = get(u, v);
        return depth[u] + depth[v] - 2*depth[lca];
    }

    int get_kth_ancestor(int u, const int K) {
        for(int i = 0; i < k; i++) {
            if(K >> i & 1) u = dp[i][u];
        }
        return u;
    }

    pair<int, int> get_distance_to_LCA(const int u, const int v) {
        int lca = get(u, v);
        return make_pair(depth[u] - depth[lca], depth[v] - depth[lca]);
    }

    //uvパス上のv_0, v_1, ... v_kに関してそのi番目を返す(k<iのときは-1)
    //0-indexedでiを指定する(0--最初)
    int jump(int u, int v, int i) {
        pair<int, int> p = get_distance_to_LCA(u, v);
        if(p.first + p.second < i) return -1;

        if(i <= p.first) return get_kth_ancestor(u, i);
        return get_kth_ancestor(v, p.first + p.second -i);
    }

    //u->lca, v->lcaパスを返す
    pair<vector<int>, vector<int>> get_path_to_LCA(int u, int v) {
        int lca = get(u, v);
        vector<int> u_ret;
        while(u != lca) {
            u_ret.push_back(u);
            u = dp[0][u];
        }
        u_ret.push_back(lca);
        vector<int> v_ret;
        while(v != lca) {
            v_ret.push_back(v);
            v = dp[0][v];
        }
        v_ret.push_back(lca);
        return make_pair(u_ret, v_ret);
    }

    //u->vパスを返す。
    vector<int> get_path(int u, int v) {
        vector<int> u_ret, v_ret;
        tie(u_ret, v_ret) = get_path_to_LCA(u, v);
        v_ret.pop_back();
        reverse(v_ret.begin(), v_ret.end());

        vector<int> ret;
        for(auto e : u_ret) ret.push_back(e);
        for(auto e : v_ret) ret.push_back(e);
        return ret;
    }
};
//add_edge version
template<typename T = int>
class LCA{
    private:
    T root;
    int n;
    int k;
    vector<vector<T>> _G;
    vector<vector<T>> dp;
    vector<T> depth;

    public:
    LCA(const int N) : n(N), _G(N) {}

    void add_edge(int u, int v) {
        _G[u].push_back(v);
        _G[v].push_back(u);
        return;
    }

    void rebuild(const int _root) {
        root = _root;
        k = 1;
        int two = 2;
        while(two < n) {
            two <<= 1;
            k++;
        }

        dp = vector<vector<T>>(k+1, vector<T>(n, -1));
        depth.resize(n);
        function<void(int, int)> _dfs = [&](int v, int p) {
            dp[0][v] = p;
            for(auto nv : _G[v]) {
                if(nv == p) continue;
                depth[nv] = depth[v] + 1;
                _dfs(nv, v);
            }
        };
        _dfs(root, -1);

        for(int i = 0; i < k; i++) {
            for(int j = 0; j < n; j++) {
                if(dp[i][j] == -1) continue;
                dp[i + 1][j] = dp[i][dp[i][j]];
            }
        }
    }

    int get(int u, int v) {
        if(depth[u] < depth[v]) swap(u, v);
        if(depth[u] != depth[v]) {
            long long d = depth[u] - depth[v];
            for(int i = 0; i < k; i++) if((d>>i) & 1) u = dp[i][u];
        }

        if(u==v) return u;

        //一致する点のひとつ前のところに移動。
        for(int i = k; i >= 0; i--) {
            if(dp[i][u] != dp[i][v]) {
                u = dp[i][u];
                v = dp[i][v];
            }
        }
        return dp[0][u];
    }

    int get_distance(const int u, const int v) {
        int lca = get(u, v);
        return depth[u] + depth[v] - 2*depth[lca];
    }

    int get_kth_ancestor(int u, const int K) {
        for(int i = 0; i < k; i++) {
            if(K >> i & 1) u = dp[i][u];
        }
        return u;
    }

    pair<int, int> get_distance_to_LCA(const int u, const int v) {
        int lca = get(u, v);
        return make_pair(depth[u] - depth[lca], depth[v] - depth[lca]);
    }

    //uvパス上のv_0, v_1, ... v_kに関してそのi番目を返す(k<iのときは-1)
    //0-indexedでiを指定する(0--最初)
    int jump(int u, int v, int i) {
        pair<int, int> p = get_distance_to_LCA(u, v);
        if(p.first + p.second < i) return -1;

        if(i <= p.first) return get_kth_ancestor(u, i);
        return get_kth_ancestor(v, p.first + p.second -i);
    }
};


//LCA using RMQ algorithm
template<typename T> class segtree {
private:
    int n,sz;
    vector<pair<T, int> > node;
public:
    void resize(const vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n);
        for(int i = 0; i < sz; i++){
            node[i+n] = make_pair(v[i], i);
        }
        for(int i=n-1; i>=1; i--){
            node[i] = min(node[2*i], node[2*i+1]);
        }
    }
    pair<T, int> query(int a,int b)
    {
        pair<T, int> res1 = make_pair(numeric_limits<T>::max(), -1);
        pair<T, int> res2 = make_pair(numeric_limits<T>::max(), -1);
        a += n, b += n;
        while(a != b){
            if(a % 2) res1 = min(res1, node[a++]);
            if(b % 2) res2 = min(res2, node[--b]);
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
};
 
class LCA{
private:
    int V;
    vector<vector<int> > G;
    vector<int> ord,depth,id;
    segtree<int> st;
    void dfs(int u,int p,int k){
        id[u] = (int)ord.size();
        ord.push_back(u);
        depth[u] = k;
        for(int v : G[u]){
            if(v != p){
                dfs(v,u,k+1);
                ord.push_back(u);
            }
        }
    }
 
public:
    LCA(int node_size) : V(node_size), G(V), depth(V), id(V, -1){}
    void add_edge(int from,int to){
        G[from].push_back(to),G[to].push_back(from);
    }
    void build(){
        ord.reserve(2*V-2);
        for(int i = 0; i < V; i++){
            if(id[i] < 0){
                dfs(i,-1,0);
            }
        }
        vector<int> stvec(2*V-2);
    	for(int i = 0; i < 2*V-2; i++){
    		stvec[i] = depth[ord[i]];
    	}
        st.resize(stvec);
    }
    int get(int u,int v){
        return ord[st.query(min(id[u],id[v]),max(id[u],id[v])+1).second];
    }
    int get_distance(int u,int v){
        int lca_ = get(u,v);
        return depth[u] + depth[v] - 2*depth[lca_];
    }
};

