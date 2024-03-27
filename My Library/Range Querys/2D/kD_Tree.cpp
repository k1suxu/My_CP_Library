using T = pair<int, int>;
struct kDTree{
    using Iter = vector<T>::iterator;
    kDTree *l = nullptr, *r = nullptr;
    int xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;
    ll lazy = 0;
    kDTree(Iter begin, Iter end, bool divx = true){
        for(auto p = begin; p != end; p++){
            auto [x, y] = *p;
            chmin(xmin, x);
            chmax(xmax, x);
            chmin(ymin, y);
            chmax(ymax, y);
        }
        const int size = int(end - begin);
        if(size <= 1) return;
        auto cen = begin + size / 2;
        if(divx){
            nth_element(begin, cen, end, [](T& a, T& b){ return a.first < b.first; });
        }
        else{
            nth_element(begin, cen, end, [](T& a, T& b){ return a.second < b.second; });
        }
        l = new kDTree(begin, cen, !divx);
        r = new kDTree(cen, end, !divx);
    }
    // [x1, x2] * [y1, y2] にある点に C を加算
    void update(int x1, int x2, int y1, int y2, int C){
        // [xmin, xmax] * [ymin, ymax] と [x1, x2] * [y1, y2] に共通部分がない
        if(x2 < xmin || xmax < x1 || y2 < ymin || ymax < y1) return;
        // [xmin, xmax] * [ymin, ymax] 全体が [x1, x2] * [y1, y2] に含まれている
        if(x1 <= xmin && xmax <= x2 && y1 <= ymin && ymax <= y2){
            // lazy += C; // range add
            lazy = C; // range update
            return;
        }
        // [xmin, xmax] * [ymin, ymax] の一部が [x1, x2] * [y1, y2] に含まれている
        l->update(x1, x2, y1, y2, C);
        r->update(x1, x2, y1, y2, C);
    }
    // [x, x] * [y, y] にある点の重みを数える
    ll get(int x1, int x2, int y1, int y2) const {
        // [xmin, xmax] * [ymin, ymax] と [x1, x2] * [y1, y2] に共通部分がない
        if(x2 < xmin || xmax < x1 || y2 < ymin || ymax < y1) return 0;
        // [xmin, xmax] * [ymin, ymax] 全体が [x1, x2] * [y1, y2] に含まれている
        if(x1 <= xmin && xmax <= x2 && y1 <= ymin && ymax <= y2) return lazy;
        // [xmin, xmax] * [ymin, ymax] の一部が [x1, x2] * [y1, y2] に含まれている -> 子に任せる
        return max({lazy, l->get(x1, x2, y1, y2), r->get(x1, x2, y1, y2)}); // range max
        // return lazy + l->get(x1, x2, y1, y2) + r->get(x1, x2, y1, y2); // range sum
    }
};

// ref : https://trap.jp/post/1489/

/* mutableな状態で配列を渡さないとダメ
auto tree = [query]() mutable {
    return kDTree(query.begin(), query.end());
}();
*/