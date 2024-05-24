template<typename flow_t, typename cost_t>
struct Edge {
    int to;
    flow_t cap;
    cost_t cost;
    int rev;
    bool isrev;

    Edge() = default;
    Edge(int to_, flow_t cap_, cost_t cost_, int rev_, bool isrev_) :
        to(to_), cap(cap_), cost(cost_), rev(rev_), isrev(isrev_) {}
};

//O(FElogV)
template<typename flow_t, typename cost_t>
struct Primal_Dual {
    using ne = Edge<flow_t, cost_t>;

    vector<vector<ne>> graph, prev_graph;
    vector<cost_t> potential, min_cost;

    vector<pair<int, int>> Edge_ID;

    vector<int> prev_v, prev_e;
    const flow_t f_zero;
    const cost_t c_zero, INF;

    Primal_Dual() = default;
    Primal_Dual(int Vs, flow_t f_zero_ = 0, cost_t c_zero_ = 0) :
        graph(Vs), f_zero(f_zero_), c_zero(c_zero_), INF(numeric_limits<cost_t>::max()) {}

    int add_edge(int from, int to, flow_t cap, cost_t cost) {
        ne from_e = {to, cap, cost, (int)graph[to].size(), false};
        ne to_e = {from, f_zero, -cost, (int)graph[from].size(), true};

        int Now_Edge_ID = (int)Edge_ID.size();
        Edge_ID.emplace_back(from, (int)graph[from].size());

        graph[from].push_back(from_e);
        graph[to].push_back(to_e);

        return Now_Edge_ID;
    }

    //return flow cost
    pair<flow_t, cost_t> min_cost_max_flow(int s, int t) {
        return min_cost_flow(s, t, std::numeric_limits<flow_t>::max());
    }
    //assert flows is sorted
    vector<pair<flow_t, cost_t>> min_cost_flow_some_flow_values(int s, int t, vector<flow_t> flows) {
        if(flows.size() == 0) return {};
        vector<pair<flow_t, cost_t>> ret = {min_cost_flow(s, t, flows[0])};
        for(int i = 0; i < flows.size() - 1; i++) {
            pair<flow_t, cost_t> now = min_cost_flow(s, t, flows[i+1] - flows[i], false);
            ret.push_back(make_pair(ret.back().first + now.first, ret.back().second + now.second));
        }
        assert(ret.size() == flows.size());
        return ret;
    }
    //s->tへ流量fの最小費用流を流す。戻り値は最小費用の値
    //s->tへfを越えない最大フローを流すときの最小費用を返す
    //first: 流量, second: コスト
    pair<flow_t, cost_t> min_cost_flow(int s, int t, flow_t f, bool remember_prev_graph = true) {
        flow_t prim_f = f;
        if(remember_prev_graph) prev_graph = graph;
        int Vs = (int)graph.size();
        cost_t ret = c_zero;
        using Pi = pair<cost_t, int>;
        priority_queue<Pi, vector<Pi>, greater<Pi>> pq;
        potential.assign(Vs, c_zero); //TODO
        prev_e.assign(Vs, -1);
        prev_v.assign(Vs, -1);

        while(f > f_zero) {
            min_cost.assign(Vs, INF);
            pq.emplace(c_zero, s);
            min_cost[s] = c_zero;

            //dijkstra
            while(!pq.empty()) {
                Pi p = pq.top();
                pq.pop();

                if(min_cost[p.second] < p.first) continue;

                for(int i = 0; i < (int)graph[p.second].size(); i++) {
                    ne &e = graph[p.second][i];
                    cost_t nxt_cost = min_cost[p.second] + e.cost + potential[p.second] - potential[e.to];

                    if(e.cap > c_zero && min_cost[e.to] > nxt_cost) {
                        min_cost[e.to] = nxt_cost;
                        prev_v[e.to] = p.second;
                        prev_e[e.to] = i;
                        pq.emplace(min_cost[e.to], e.to);
                    }
                }
            }

            //Unreachable
            if(min_cost[t] == INF) {
                break;
            }
            for(int v = 0; v < Vs; v++) potential[v] += min_cost[v];
            flow_t add_flow = f;

            for(int v = t; v != s; v = prev_v[v]) {
                add_flow = min(add_flow, graph[prev_v[v]][prev_e[v]].cap);
            }

            f -= add_flow;

            ret += add_flow * potential[t];

            for(int v = t; v != s; v = prev_v[v]) {
                ne &e = graph[prev_v[v]][prev_e[v]];
                e.cap -= add_flow;
                graph[v][e.rev].cap += add_flow;
            }
        }

        return make_pair(prim_f-f, ret);
    }

    vector<pair<flow_t, cost_t>> slope_all_range(int s, int t) {
        return slope(s, t, f_zero, std::numeric_limits<flow_t>::max());
    }
    vector<pair<flow_t, cost_t>> slope_to_max(int s, int t, flow_t f_min) {
        return slope(s, t, f_min, std::numeric_limits<flow_t>::max());
    }    
    //s->tに流量 f_min~min(最大流量, f_max) を流した時のコスト(折り返しになるところのみ)。
    //(コスト関数f:flow_t->cost_tは任意の点においてf'(x)=cなる定数を返すが,
    //そのcが変わる点の情報のみを保持する(c=cost_t_dif/flow_t_difなので,
    //c=(slope[i+1].second-slope[i].second)/(slope[i+1].flow-slope[i].flow)で求められる))
    vector<pair<flow_t, cost_t>> slope(int s, int t, flow_t f_min, flow_t f_max) {
        prev_graph = graph;
        pair<flow_t, cost_t> f_min_flow = min_cost_flow(s, t, f_min);
        if(f_min_flow.first != f_min) return {};

        flow_t remained_flow = f_max - f_min;
        vector<pair<flow_t, cost_t>> slope_result = {f_min_flow};

        int Vs = (int)graph.size();
        cost_t ret = c_zero;
        using Pi = pair<cost_t, int>;
        priority_queue<Pi, vector<Pi>, greater<Pi>> pq;
        potential.assign(Vs, c_zero); //TODO
        prev_e.assign(Vs, -1);
        prev_v.assign(Vs, -1);

        while(remained_flow > f_zero) {
            min_cost.assign(Vs, INF);
            pq.emplace(c_zero, s);
            min_cost[s] = c_zero;

            //dijkstra
            while(!pq.empty()) {
                Pi p = pq.top();
                pq.pop();

                if(min_cost[p.second] < p.first) continue;

                for(int i = 0; i < (int)graph[p.second].size(); i++) {
                    ne &e = graph[p.second][i];
                    cost_t nxt_cost = min_cost[p.second] + e.cost + potential[p.second] - potential[e.to];

                    if(e.cap > c_zero && min_cost[e.to] > nxt_cost) {
                        min_cost[e.to] = nxt_cost;
                        prev_v[e.to] = p.second;
                        prev_e[e.to] = i;
                        pq.emplace(min_cost[e.to], e.to);
                    }
                }
            }

            //Unreachable
            if(min_cost[t] == INF) {
                break;
            }
            for(int v = 0; v < Vs; v++) potential[v] += min_cost[v];
            flow_t add_flow = remained_flow;

            //差分更新だからgraph[prev_v[v]][prev_e[v]].cap-before_graph[prev_v[v]][prev_e[v]].capでは？？
            for(int v = t; v != s; v = prev_v[v]) {
                add_flow = min(add_flow, graph[prev_v[v]][prev_e[v]].cap);
            }

            remained_flow -= add_flow;

            //差分更新だからadd_flow * (potential[t]-before_potential[t])では？？
            ret += add_flow * potential[t];

            slope_result.push_back(make_pair(slope_result.back().first+add_flow, ret));
            // 全点列挙なら下だけど、端点列挙なら上だけでいいのでは??
            // for(int i = 0; i < add_flow; i++) {
            //     slope_result.push_back(make_pair(slope_result.back().first + 1, slope_result.back().second + potential[t]));
            // }

            for(int v = t; v != s; v = prev_v[v]) {
                ne &e = graph[prev_v[v]][prev_e[v]];
                e.cap -= add_flow;
                graph[v][e.rev].cap += add_flow;
            }
        }

        return slope_result;
    }

    //グラフをフローを流す前に戻す
    void recover() {
        graph = prev_graph;
    }

    //0-indexed
    //assert recovered
    void update_cap(int i, flow_t new_cap) {
        int from = Edge_ID[i].first;
        int from_g_id = Edge_ID[i].second;
        auto& e = graph[from][from_g_id];
        e.cap = new_cap;
        graph[e.to][e.rev].cap = 0;
    }
    //don't have to assert recovered
    void update_cost(int i, cost_t new_cost) {
        int from = Edge_ID[i].first;
        int from_g_id = Edge_ID[i].second;
        auto& e = graph[from][from_g_id];
        e.cost = new_cost;
        graph[e.to][e.rev].cost = -new_cost;
    }
    //assert recovered (because of update_cap);
    void update(int i, flow_t new_cap, cost_t new_cost) {
        update_cap(i, new_cap);
        update_cost(i, new_cost);
    }

    //don't have to assert recovered
    void add_cap(int i, flow_t add_cap) {
        //don't have to assert recoveredにしたい！！
        int from = Edge_ID[i].first;
        int from_g_id = Edge_ID[i].second;
        auto& e = graph[from][from_g_id];
        e.cap = e.cap + add_cap;
        graph[e.to][e.rev].cap = graph[e.to][e.rev].cap;
    }
    //don't have to assert recovered
    void add_cost(int i, cost_t add_cost) {
        update_cost(i, graph[Edge_ID[i].first][Edge_ID[i].second].cost + add_cost);
    }
    //first: from, second: (to, cap, cost, rev, isrev)
    pair<int, ne> get_ith_edge(int i) {
        return make_pair(Edge_ID[i].first, graph[Edge_ID[i].first][Edge_ID[i].second]);
    }

    void print() {
        for(int i = 0; i < (int)graph.size(); i++) {
            for(ne e : graph[i]) {
                if(e.isrev) continue;
                ne rev_e = graph[e.to][e.rev];

                cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << rev_e.cap + e.cap << ")" << " cost=" << rev_e.cap * e.cost << endl;
            }
        }
    }
};
//二回以上流す(slopeは一回としてカウント)時は、必ずrecover()をすること！

//functions
/*
[Verified]


[Uncertain]


*/
//abc247を参考に端点列挙ができてるかverifyすること!!