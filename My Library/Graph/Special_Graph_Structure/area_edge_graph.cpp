//区間に辺を張るテクの二次元バージョン
//T: 重みのデータの型
template<typename T>
struct area_edge_graph {
public:
    int id(int h, int w) { return h * 2 * W + w; }
    int id_sub(int h, int w) {
        if(H <= h && W <= w) return id(h, w);
        return 8 * H * W - id(h, w);
    }


private:
    void get_size_of_graph(int h, int w) {
        H = W = 1;
        while (H < h) H <<= 1;
        while (W < w) W <<= 1;
        g.resize(8 * H * W);
    }

    void build_binary_tree() {
        // w in [W, 2W)
        for (int w = W; w < 2 * W; w++) {
            for (int h = H - 1; h; h--) {
                add_edge_single_to_single_sub(id(h, w), id(2 * h + 0, w), 0);
                add_edge_single_to_single_sub(id(h, w), id(2 * h + 1, w), 0);
                add_edge_single_to_single_sub(id_sub(2 * h + 0, w), id_sub(h, w), 0);
                add_edge_single_to_single_sub(id_sub(2 * h + 1, w), id_sub(h, w), 0);
            }
        }
        // h in [0, 2H)
        for (int h = 0; h < 2 * H; h++) {
            for (int w = W - 1; w; w--) {
                add_edge_single_to_single_sub(id(h, w), id(h, 2 * w + 0), 0);
                add_edge_single_to_single_sub(id(h, w), id(h, 2 * w + 1), 0);
                add_edge_single_to_single_sub(id_sub(h, 2 * w + 0), id_sub(h, w), 0);
                add_edge_single_to_single_sub(id_sub(h, 2 * w + 1), id_sub(h, w), 0);
            }
        }
    }

    void add_edge_single_to_single_sub(int u, int v, T w) {
        g[u].emplace_back(v, w);
    }

    void add_edge_sub1(int from, int h, int w1, int w2, T weight) {
        for (; w1 < w2; w1 >>= 1, w2 >>= 1) {
            if (w1 & 1) add_edge_single_to_single_sub(from, id(h, w1), weight), w1++;
            if (w2 & 1) --w2, add_edge_single_to_single_sub(from, id(h, w2), weight);
        }
    }

    // undebugged
    void add_edge_sub2(int h, int w1, int w2, int to, T weight) {
        for (; w1 < w2; w1 >>= 1, w2 >>= 1) {
            if (w1 & 1) add_edge_single_to_single_sub(id_sub(h, w1), to, weight), w1++;
            if (w2 & 1) --w2, add_edge_single_to_single_sub(id_sub(h, w2), to, weight);
        }
    }

    void add_edge_single_to_area_sub(int from, int h1, int h2, int w1, int w2, T weight) {
        h1 += H, h2 += H, w1 += W, w2 += W;
        for (; h1 < h2; h1 >>= 1, h2 >>= 1) {
            if (h1 & 1) add_edge_sub1(from, h1, w1, w2, weight), h1++;
            if (h2 & 1) --h2, add_edge_sub1(from, h2, w1, w2, weight);
        }
    }
    // undebugged
    void add_edge_area_to_single_sub(int h1, int h2, int w1, int w2, int to, T weight) {
        h1 += H, h2 += H, w1 += W, w2 += W;
        for (; h1 < h2; h1 >>= 1, h2 >>= 1) {
            if (h1 & 1) add_edge_sub2(h1, w1, w2, to, weight), h1++;
            if (h2 & 1) --h2, add_edge_sub2(h2, w1, w2, to, weight);
        }
    }
public:
    int H, W;
    vector<vector<pair<int, T>>> g;

    area_edge_graph(int h, int w) {
        get_size_of_graph(h, w);
        build_binary_tree();
    }

    int get_single_node_id(int h, int w) { return id(h + H, w + W); }

    void add_edge_single_to_single(int from_x, int from_y, int to_x, int to_y, T weight) {
        add_edge_single_to_single_sub(get_single_node_id(from_x, from_y), get_single_node_id(to_x, to_y), weight);
    }

    // [ (h1,w1), (h2,w2) ) 半開
    void add_edge_single_to_area(int from_x, int from_y, int h1, int h2, int w1, int w2, T weight) {
        if (h1 >= h2 || w1 >= w2) return;
        add_edge_single_to_area_sub(get_single_node_id(from_x, from_y), h1, h2, w1, w2, weight);
    }

    // undebugged
    void add_edge_area_to_single(int h1, int h2, int w1, int w2, int to_x, int to_y, T weight) {
        if (h1 >= h2 || w1 >= w2) return;
        add_edge_area_to_single_sub(h1, h2, w1, w2, get_single_node_id(to_x, to_y), weight);
    }
    // undebugged
    void add_edge_area_to_area(int from_h1, int from_h2, int from_w1, int from_w2, int to_h1, int to_h2, int to_w1, int to_w2, T weight) {
        if (from_h1 >= from_h2 || from_w1 >= from_w2 || to_h1 >= to_h2 || to_w1 >= to_w2) return;

        int super1 = g.size(), super2 = super1+1;
        g.push_back(vector<pair<int, T>>());
        g.push_back(vector<pair<int, T>>());

        add_edge_area_to_single_sub(from_h1, from_h2, from_w1, from_w2, super1, 0);
        add_edge_single_to_single_sub(super1, super2, weight);
        add_edge_single_to_area_sub(super2, to_h1, to_h2, to_w1, to_w2, 0);
    }

    vector<T> bfs(int stx, int sty) {
        int st = get_single_node_id(stx, sty);
        queue<int> que;
        vector<int> dist(g.size(), -1);
        que.push(st);
        dist[st] = 0;
        while(!que.empty()) {
            int v = que.front();
            que.pop();
            for(auto e : g[v]) {
                if(dist[e.first] < 0) {
                    dist[e.first] = dist[v] + 1;
                    que.push(e.first);
                }
            }
        }
        return dist;
    }

    vector<T> bfs_01(int stx, int sty) {
        int st = get_single_node_id(stx, sty);
        vector<T> dist(g.size(), INF);
        vector<bool> done(g.size(), false);
        dist[st] = 0;
        deque<int> que;
        que.push_back(st);

        while(!que.empty()) {
            int v = que.front();
            que.pop_front();

            if(done[v]) continue;
            done[v] = true;

            for(auto &e : g[v]) {
                if(dist[v]+e.second < dist[e.first]) {
                    if(e.second == 0) {
                        dist[e.first] = dist[v];
                        que.push_front(e.first);
                    }else {
                        assert(e.second == 1);
                        dist[e.first] = dist[v] + 1;
                        que.push_back(e.first);
                    }
                }
            }
        }

        return dist;
    }

    vector<T> dijkstra(int stx, int sty) {
        int st = get_single_node_id(stx, sty);
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
        vector<int> dist(g.size(), INF);
        dist[st] = 0;
        pq.emplace(dist[st], st);
        while(!pq.empty()) {
            pair<T, int> p = pq.top();
            pq.pop();
            int v = p.second;
            if(dist[v] < p.first) continue;
            for(auto e : g[v]) {
                if(dist[e.first] > dist[v] + e.second) {
                    dist[e.first] = dist[v] + e.second;
                    pq.emplace(dist[e.first], e.first);
                }
            }
        }
        return dist;
    }
};

// 区間幅が固定の場合、logを取れる(noshiさんのツイート参照)
// 点→区間　の操作しか必要ない場合はセグ木の下側を消してもいいので2倍くらい速くなる