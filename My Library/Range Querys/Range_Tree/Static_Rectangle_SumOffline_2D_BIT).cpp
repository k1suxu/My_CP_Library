template< typename T >
struct BinaryIndexedTree {
private:
    int n;
    vector< T > data;

public:
    BinaryIndexedTree() = default;

    explicit BinaryIndexedTree(int n) : n(n) {
        data.assign(n + 1, 0);
    }

    explicit BinaryIndexedTree(const vector< T > &v) :
        BinaryIndexedTree((int) v.size()) {
        build(v);
    }

    void build(const vector< T > &v) {
        assert(n == (int) v.size());
        for(int i = 1; i <= n; i++) data[i] = v[i - 1];
        for(int i = 1; i <= n; i++) {
            int j = i + (i & -i);
            if(j <= n) data[j] += data[i];
        }
    }

    void apply(int k, const T &x) {
        for(++k; k <= n; k += k & -k) data[k] += x;
    }

    T prod(int r) const {
        T ret = T();
        for(; r > 0; r -= r & -r) ret += data[r];
        return ret;
    }

    T prod(int l, int r) const {
        return prod(r) - prod(l);
    }

    int lower_bound(T x) const {
        int i = 0;
        for(int k = 1 << (__lg(n) + 1); k > 0; k >>= 1) {
            if(i + k <= n && data[i + k] < x) {
                x -= data[i + k];
                i += k;
            }
        }
        return i;
    }

    int upper_bound(T x) const {
        int i = 0;
        for(int k = 1 << (__lg(n) + 1); k > 0; k >>= 1) {
            if(i + k <= n && data[i + k] <= x) {
                x -= data[i + k];
                i += k;
            }
        }
        return i;
    }
};

template< typename Coordinate, typename Value_Type >
struct Static_2D_BIT {
    using BIT = BinaryIndexedTree< Value_Type >;

    static_assert(is_integral< Coordinate >::value, "template parameter Coordinate must be integral type");

    struct Point {
        Coordinate x, y;
        Value_Type w;
    };

    struct Query {
        Coordinate l, d, r, u;
    };

    vector< Point > points;
    vector< Query > queries;

    Static_2D_BIT() = default;

    void add_point(Coordinate x, Coordinate y, Value_Type w) {
        points.emplace_back(Point{x, y, w});
    }

    // tatal weight of [l, r) * [d, u) points
    void add_query(Coordinate l, Coordinate r, Coordinate d, Coordinate u) {
        queries.emplace_back(Query{l, d, r, u});
    }

    vector< Value_Type > calculate_queries() {
        int n = (int) points.size();
        int q = (int) queries.size();

        sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.y < b.y;
        });
        vector< Coordinate > ys;
        ys.reserve(n);
        for(Point &p: points) {
            if(ys.empty() or ys.back() != p.y) ys.emplace_back(p.y);
            p.y = (int) ys.size() - 1;
        }
        ys.shrink_to_fit();

        struct Q {
            Coordinate x, d, u;
            bool type;
            int idx;
        };
        vector< Q > qs;
        qs.reserve(q + q);
        for(int i = 0; i < q; i++) {
            auto &query = queries[i];
            int d = lower_bound(ys.begin(), ys.end(), query.d) - ys.begin();
            int u = lower_bound(ys.begin(), ys.end(), query.u) - ys.begin();
            qs.emplace_back(Q{query.l, d, u, false, i});
            qs.emplace_back(Q{query.r, d, u, true, i});
        }
        sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.x < b.x;
        });
        sort(qs.begin(), qs.end(), [](const Q &a, const Q &b) {
            return a.x < b.x;
        });
        vector< Value_Type > ans(q);
        int j = 0;
        BIT bit(ys.size());
        for(auto &query: qs) {
            while(j < n and points[j].x < query.x) {
                bit.apply(points[j].y, points[j].w);
                ++j;
            }
            if(query.type) ans[query.idx] += bit.prod(query.d, query.u);
            else ans[query.idx] -= bit.prod(query.d, query.u);
        }
        return ans;
    }
};

//ref: https://ei1333.github.io/library/other/static-point-add-rectangle-sum.hpp

// 点追加が全て終わった後の盤面に対してのみ矩形和クエリが走る
// 点追加と矩形和取得が同時に行われるときはonline(dynamic)な方のRectangle Sumを使う