template<class S, S (*op)(S, S), S (*e)()>
struct SparseTable {
    vector<vector<S>> st;
    vector<int> lookup;

    SparseTable() = default;
    SparseTable(const vector<S> &v) {
        int b = 0;
        while((1 << b) <= v.size()) ++b;
        st.assign(b, vector<S>(1 << b));
        for(int i = 0; i < v.size(); i++) {
            st[0][i] = v[i];
        }
        for(int i = 1; i < b; i++) {
            for(int j = 0; j + (1 << i) <= (1 << b); j++) {
                st[i][j] = op(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
        lookup.resize(v.size() + 1);
        for(int i = 2; i < lookup.size(); i++) {
            lookup[i] = lookup[i >> 1] + 1;
        }
    }

    inline S query(int l, int r) {
        int b = lookup[r - l];
        return op(st[b][l], st[b][r - (1 << b)]);
    }
};