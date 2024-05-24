template<class S, S (*op)(S, S), S (*e)()>
struct Doubling {
    int N, LOG;
    vector<vector<S>> doubling;
    Doubling(int N, long long to_max) : N(N) {
        long long k = 1;
        LOG = 1;
        while(k<to_max) {
            k *= 2;
            ++LOG;
        }
        doubling.resize(LOG, vector<S>(N, e()));
    }
    Doubling(int N, long long to_max, vector<S> x) : N(N) {
        long long k = 1;
        LOG = 1;
        while(k<to_max) {
            k *= 2;
            ++LOG;
        }
        doubling.resize(LOG, vector<S>(N, e()));
        doubling[0] = x;
    }
    void init(vector<S> x) {
        doubling[0] = x;
    }
    void set_i(int i, S x) {
        doubling[0][i] = x;
    }

    void bin_calculate() {
        for(int k = 0; k < LOG-1; k++) {
            for(int i = 0; i < N; i++) {
                doubling[k+1][i] = op(doubling[k][i], doubling[k][doubling[k][i].to]);
            }
        }
    }

    //from ith node, move k times
    S get(int i, long long k) {
        int npos = i;
        S cur_sum = e();
        for(int bit = 0; bit < LOG; bit++) {
            if(k>>bit&1) {
                cur_sum = op(cur_sum, doubling[bit][npos]);
                npos = doubling[bit][npos].to;                
            }
        }
        return cur_sum;
    }
};

//S二は必ず行き先のindexを示す"to"が必要
//e()のtoは-1を返す(opでassertしてる)
//モノイドを載せる(vector<bool> updとかを持てば半群としても処理できそうだけどまあ今のところはモノイドでいいでしょ)

struct S {
    int to, sum, max;
    S(int to, int sum, int max) : to(to), sum(sum), max(max) {}
};
S op(S x, S y) {
    assert(y.to != -1);
    return S(y.to, x.sum+y.sum, max(x.max, x.sum+y.max));
}
S e() {
    return S(-1, 0, -INF);
}
//ref problem sample : https://atcoder.jp/contests/abc175/submissions/41263724
//グラフ上でのダブリングにしか使えない