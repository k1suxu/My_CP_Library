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
        Edge_ID.emplace_back(from, (int)g[from].size());

        graph[from].push_back(from_e);
        graph[to].push_back(to_e);

        return Now_Edge_ID;
    }
    int add_edge(npe edge) {
        return add_edge(edge.from, edge.to, edge.cap, edge.cost);
    }

    pair<flow_t, cost_t> min_cost_max_flow(int s, int t) {
        return min_cost_flow(s, t, std::numeric_limits<flow_t>::max());
    }
    //assert flows is sorted
    vector<pair<flow_t, cost_t>> min_cost_flow_some_flow_value(int s, int t, vector<flow_t> flows) {
        if(flows.size() == 0) return {};
        vector<pair<flow_t, cost_t>> ret = min_cost_flow(s, t, flows.front());
        for(int i = 0; i < flows.size() - 1; i++) {
            pair<flow_t, cost_t> now = min_cost_flow(s, t, flows[i+1] - flows[i]);
            ret.push_back(make_pair(ret.back().first + now.first, ret.back().second + now.second));
        }
        assert(ret.size() == flows.size());
        return ret;
    }
    //s->t�֗���f�̍ŏ���p���𗬂��B�߂�l�͍ŏ���p�̒l
    //s->t��f���z���Ȃ��ő�t���[�𗬂��Ƃ��̍ŏ���p��Ԃ�
    //first: ����, second: �R�X�g
    pair<flow_t, cost_t> min_cost_flow(int s, int t, flow_t f) {
        flow_t prim_f = f;
        prev_graph = graph;
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
    //s->t�ɗ��� f_min~min(�ő嗬��, f_max) �𗬂������̃R�X�g��Ԃ��B
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

            for(int v = t; v != s; v = prev_v[v]) {
                add_flow = min(add_flow, graph[prev_v[v]][prev_e[v]].cap);
            }

            remained_flow -= add_flow;

            ret += add_flow * potential[t];

            for(int i = 0; i < add_flow; i++) {
                slope_result.push_back(make_pair(slope_result.back().first + 1, slope_result.back().second + potential[t]));
            }

            for(int v = t; v != s; v = prev_v[v]) {
                ne &e = graph[prev_v[v]][prev_e[v]];
                e.cap -= add_flow;
                graph[v][e.rev].cap += add_flow;
            }
        }

        return slope_result;
    }

    //�O���t���t���[�𗬂��O�ɖ߂�
    void recover() {
        graph = prev_graph;
    }

    //0-indexed
    //�t���[�͗������܂܍X�V����(Req: ���̃O���t��ω����������ꍇ�͂܂�recover()���邱�ƁI�I�I)
    void update_cap(int i, flow_t new_cap) {
        auto& e = graph[Edge_ID[i].first][Edge_ID[i].second];
        e.cap = new_cap;
    }
    void update_cost(int i, cost_t new_cost) {
        auto& e = graph[Edge_ID[i].first][Edge_ID[i].second];
        e.cost = new_cost;
        auto& rev_e = graph[e.to][e.rev];
        rev_e.cost = -new_cost;
    }
    void update(int i, flow_t new_cap, cost_t new_cost) {
        update_cap(i, new_cap);
        update_cost(i, new_cost);
    }

    //add cap
    void add_cap(int i, flow_t add_cap) {
        update_cap(i, graph[Edge_ID[i].fi][Edge_ID[i].second].cap + add_cap);
    }
    //add cost
    void add_cost(int i, cost_t add_cost) {
        update_cap(i, graph[Edge_ID[i].fi][Edge_ID[i].second].cost + add_cost);
    }
    //get ith edge 
    pair<int, ne> get_ith_edge(int i) {
        return make_pair(Edge_ID[i].first, graph[Edge_ID[i].first][Edge_ID[i].second]);
    }
    //get all edges
    vector<pair<int, ne>> get_all_edges() {
        vector<pair<int, ne>> ret;
        for(int i = 0; i < (int)Edge_ID.size(); i++) ret.push_back(get_ith_edge(i));
        return ret;
    }

    void print() {
        for(int i = 0; i < (int)graph.size(); i++) {
            for(ne &e : graph[i]) {
                if(e.isrev) continue;
                ne &rev_e = graph[e.to][e.rev];

                cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << rev_e.cap + e.cap << ")" << " cost=" << rev_e.cap * e.cost << endl;
            }
        }
    }
};
//flow�̈�����recover?�̃u�[���l���������Ċ֐�����recover���������B
//add_edge��int�̕Ԃ�l���������āAid = add_edge(), get_edge(id) = ���̕Ӂ@�݂����ɂ������B
//update_cap, add_cap -> �t���[�͗������܂܍X�V����(Req: ���̃O���t��ω����������ꍇ�͂܂�recover()���邱�ƁI�I�I)
//slope���������C������