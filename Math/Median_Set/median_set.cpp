//中央値の定義に注意
//get_median(bool bigger) -> 偶数個の要素があったときに中央2つの内どっちかを中央値とする(bigger=false->小さい方, bigger=true->大きい方)
//get_median_merge() -> 偶数個の要素があったときに中央2つをmerge(L, R)したものを中央値とする
template<typename T>
struct median_set {
private:
    multiset<T> left;
    multiset<T> right;

    void recal_size() {
        while(left.size() > right.size()+1) {
            right.insert(*prev(left.end()));
            left.erase(prev(left.end()));
        }
        while(left.size() < right.size()) {
            left.insert(*right.begin());
            right.erase(right.begin());
        }
    }
public:
    median_set() = default;

    void insert(int x) {
        if(left.size() == 0) left.insert(x);
        else if(x <= *prev(left.end())) left.insert(x);
        else right.insert(x);
        recal_size();
    }

    void erase_one(int x) {
        if(x <= *prev(left.end())) left.erase(left.find(x));
        else right.erase(right.find(x));
        recal_size();
    }

    T get_median(bool bigger) {
        if(left.size() > right.size()) return *prev(left.end());
        else if(!bigger) return *prev(left.end());
        return *right.begin();
    }
    template<T (*merge)(T, T)>
    T get_median_merge() {
        if(left.size() > right.size()) return *prev(left.end());
        return merge(*prev(left.end()), *right.begin());
    }
    template<typename Function_Type>
    T get_median_merge(Function_Type merge) {
        if(left.size() > right.size()) return *prev(left.end());
        return merge(*prev(left.end()), *right.begin());
    }
    void debug() {
        cerr << "[Median Set Debugger]\n";
        cerr << "L: ";
        for(auto e : left) cerr << e << " "; cerr << "\n";
        cerr << "R: ";
        for(auto e : right) cerr << e << " "; cerr << "\n";
    }
};

// グローバル関数でmergeを宣言するときは、定義名を`merge`以外にしないと
// internal compiler error: in tsubst_copy, at ```が出るので注意