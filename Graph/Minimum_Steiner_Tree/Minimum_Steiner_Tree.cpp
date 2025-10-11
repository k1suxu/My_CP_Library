template<typename T>
struct Minimum_Steiner_Tree {
private:
    int n;
    vector<vector<pair<int, T>>> g;
    vector<vector<T>> dp;
    static const T inf = numeric_limits<T>::max() / 10;

public:
    explicit Minimum_Steiner_Tree(const int &_n) : n(_n), g(_n) {}
    explicit Minimum_Steiner_Tree(const vector<vector<pair<int, T>>> &g) : n((int)g.size()), g(g) {}

    // 重み付き無効辺の追加
    void add_edge(const int &u, const int &v, const T &cost) {
        g[u].emplace_back(v, cost);
        g[v].emplace_back(u, cost);
    }

    T solve(const vector<int> &subset) {
        int sz = (int)subset.size();
        if(sz == 0) return (T)0;

        dp.resize(1<<sz, vector<T>(n, inf));

        for(int i = 0; i < sz; i++) dp[1<<i][subset[i]] = 0;
        
        for(int i = 1; i < (1<<sz); i++) {
            for(int j = 0; j < n; j++) {
                for(int k = i; k > 0; k = (k - 1) & i) {
                    dp[i][j] = min(dp[i][j], dp[k][j] + dp[i ^ k][j]);
                }
            }

            // 下の行をコメントアウトすることによってterminal + 1頂点の木すべての最小シュタイナー木のコストが求められる
            // if(i == (1<<sz) - 1) break;
            
            priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;

            for(int j = 0; j < n; j++) pq.push(make_pair(dp[i][j], j));

            while(!pq.empty()) {
                pair<T, int> p = pq.top();
                pq.pop();
                int v = p.second;

                if(dp[i][v] < p.first) continue;

                for(auto &e : g[v]) {
                    if(dp[i][e.first] > dp[i][v] + e.second) {
                        dp[i][e.first] = dp[i][v] + e.second;
                        pq.push(make_pair(dp[i][e.first], e.first));
                    }
                }
            }
        }

        return dp[(1<<sz) - 1][subset[0]];
    }

    vector<vector<T>> get_dp_table() {
        return dp;
    }
};

//ref: https://kopricky.github.io/code/Academic/steiner_tree.html