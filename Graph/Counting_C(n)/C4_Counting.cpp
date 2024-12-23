//長さ4の閉路の個数を計算するO(M^{1.5})
class CountingC4 {
private:
    int V, threshold;
    vector<vector<int> > G;
    vector<vector<array<int, 2> > > memo;
    vector<int> flag1, flag2;
    vvi f1, f2;
    void process_high_degree(long long& ans){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() <= threshold) continue;
            for(const int u : G[i]){
                if(u > i) flag1[u] = 1;
                flag2[u] = 1;
            }
            for(int j = 0; j < i; ++j){
                if((int)G[j].size() > threshold) continue;
                long long cnt1 = 0, cnt2 = 0;
                for(const int u : G[j]){
                    if(u < j || !flag2[u]) continue;
                    if((int)G[u].size() > threshold) ++cnt1;
                    else ++cnt2; 
                }
                ans += (cnt1 + cnt2) * (cnt1 + cnt2 - 1) / 2;
            }
            for(int j = i + 1; j < V; ++j){
                long long cnt = 0;
                for(const int u : G[j]){
                    if(flag1[u]) ++cnt;
                }
                ans += cnt * (cnt - 1) / 2;
            }
            for(const int u : G[i]) flag1[u] = flag2[u] = 0;
        }
    }
    void process_low_degree(long long& ans){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() > threshold) continue;
            for(const int u : G[i]){
                for(const int v : G[i]){
                    if(v > u) memo[u].push_back({i, v});
                }
            }
        }
        for(int i = 0; i < V; ++i){
            for(const auto& e : memo[i]){
                if(e[0] < i) ++flag1[e[1]];
                else ++flag2[e[1]];
            }
            for(const auto& e : memo[i]){
                ans += (long long)(flag1[e[1]] + 2 * flag2[e[1]] - 1) * flag1[e[1]] / 2;
                flag1[e[1]] = flag2[e[1]] = 0;
            }
        }
    }

    void process_high_degree_finding(array<int, 4> &ans){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() <= threshold) continue;
            for(const int u : G[i]){
                if(u > i) f1[u].emplace_back(i);
                f2[u].emplace_back(i);
            }
            for(int j = 0; j < i; ++j){
                if((int)G[j].size() > threshold) continue;
                vi verts;
                for(const int u : G[j]){
                    if(u < j || f2[u].empty()) continue;
                    verts.emplace_back(u);
                }
                if((int)verts.size() >= 2) {
                    ans = {i, j, verts[0], verts[1]};
                    for(const int u : G[i]) {
                        f1[u].clear();
                        f2[u].clear();
                    }
                    return;
                }
            }
            for(int j = i + 1; j < V; ++j){
                vi verts;
                for(const int u : G[j]){
                    if(!f1[u].empty()) verts.emplace_back(u);
                }
                if((int)verts.size() >= 2) {
                    ans = {i, j, verts[0], verts[1]};
                    for(const int u : G[i]) {
                        f1[u].clear();
                        f2[u].clear();
                    }
                    return;
                }
            }
            for(const int u : G[i]) {
                f1[u].clear();
                f2[u].clear();
            }
        }
    }
    void process_low_degree_finding(array<int, 4>& ans){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() > threshold) continue;
            for(const int u : G[i]){
                for(const int v : G[i]){
                    if(v > u) memo[u].push_back({i, v});
                }
            }
        }
        f1.assign(V, vector<int>());
        f2.assign(V, vector<int>());
        for(int i = 0; i < V; ++i){
            for(const auto& e : memo[i]){
                if(e[0] < i) f1[e[1]].emplace_back(e[0]);
                else f2[e[1]].emplace_back(e[0]);
            }
            for(const auto& e : memo[i]){
                if((int)f1[e[1]].size() >= 2) {
                    ans = {i, e[1], f1[e[1]][0], f1[e[1]][1]};
                    return;
                }
                if((int)f1[e[1]].size() > 0 && (int)f2[e[1]].size() > 0) {
                    ans = {i, e[1], f1[e[1]][0], f2[e[1]][0]};
                    return;
                }
                f1[e[1]].clear();
                f2[e[1]].clear();
            }
        }
    }
public:
    CountingC4(const int node_size)
         : V(node_size), threshold(0), G(V) {}
    void add_edge(const int u, const int v){
        G[u].push_back(v), G[v].push_back(u);
        ++threshold;
    }
    long long count_c4(){
        memo.assign(V, vector<array<int, 2>>());
        flag1.assign(V, 0);
        flag2.assign(V, 0);

        threshold = floor(sqrt(2 * threshold)) / 2;
        long long ans = 0;
        process_high_degree(ans);
        process_low_degree(ans);
        return ans;
    }
    array<int, 4> find_c4() {
        memo.assign(V, vector<array<int, 2>>());
        f1.assign(V, vector<int>());
        f2.assign(V, vector<int>());
        threshold = floor(sqrt(2 * threshold)) / 2;
        array<int, 4> ans = {-1, -1, -1, -1};
        process_high_degree_finding(ans);
        if(ans[0] != -1) {
            sort(all(ans));
            return ans;
        }
        process_low_degree_finding(ans);
        sort(all(ans));
        return ans;
    }
};