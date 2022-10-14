//ref: https://ei1333.github.io/algorithm/namori.html
template< typename G >
struct Namori
{
    const G &g;
    vector< int > in;

    Namori(const G &g) : g(g), in(g.size(), 0) {}

    void decomposition(vector< int > &loop, vector< vector< int > > &forest)
    {
        int N = (int) in.size();
        for(int i = 0; i < N; i++) {
            in[i] = g[i].size();
        }
        forest.resize(N);
        queue< int > que;
        vector< bool > v(N, 0);
        for(int i = 0; i < N; i++) {
            if(in[i] == 1) {
                que.emplace(i);
                v[i] = true;
            }
        }
        while(!que.empty()) {
            int idx = que.front();
            que.pop();
            for(auto &to : g[idx]) {
                if(v[to]) continue;
                --in[to];
                forest[to].push_back(idx);
                forest[idx].push_back(to);
                if(in[to] > 1) continue;
                que.emplace(to);
                v[to] = true;
            }
        }

        function< void(int) > dfs = [&](int idx) {
            loop.push_back(idx);
            for(auto &to : g[idx]) {
                if(v[to]) continue;
                v[to] = true;
                dfs(to);
            }
        };
        for(int i = 0; i < N; i++) {
            if(v[i]) continue;
            v[i] = true;
            dfs(i);
            break;
        }
    }
};
