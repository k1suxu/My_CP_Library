//LCA using RMQ algorithm
template< typename T >
struct SparseTable {
    vector< vector< T > > st;
    vector< int > lookup;

    SparseTable() = default;
    SparseTable(const vector< T > &v) {
        int b = 0;
        while((1 << b) <= v.size()) ++b;
        st.assign(b, vector< T >(1 << b));
        for(int i = 0; i < v.size(); i++) {
            st[0][i] = v[i];
        }
        for(int i = 1; i < b; i++) {
            for(int j = 0; j + (1 << i) <= (1 << b); j++) {
                st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
        lookup.resize(v.size() + 1);
        for(int i = 2; i < lookup.size(); i++) {
            lookup[i] = lookup[i >> 1] + 1;
        }
    }

    inline T rmq(int l, int r) {
        int b = lookup[r - l];
        return min(st[b][l], st[b][r - (1 << b)]);
    }
};
 
class LCA{
private:
    int V;
    vector<int> ord,id;
    SparseTable<pair<int, int>> st;
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
    vector<vector<int>> G;
    vector<int> depth;
    LCA(int node_size) : V(node_size), G(V), depth(V), id(V, -1) {}
    void add_edge(int from,int to){
        G[from].push_back(to),G[to].push_back(from);
    }
    void build(int r = 0){
        ord.reserve(2*V-2);
        dfs(r, -1, 0);
        vector<pair<int, int>> stvec(2*V-2);
    	for(int i = 0; i < 2*V-2; i++){
    		stvec[i] = make_pair(depth[ord[i]], i);
    	}
        st = SparseTable<pair<int, int>>(stvec);
    }
    int lca(int u,int v){
        return ord[st.rmq(min(id[u],id[v]),max(id[u],id[v])+1).second];
    }
    int get_distance(int u,int v){
        int lca_ = lca(u,v);
        return depth[u] + depth[v] - 2*depth[lca_];
    }
};