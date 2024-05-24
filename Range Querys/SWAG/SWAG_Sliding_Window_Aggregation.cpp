// 両端queueのような感じで(前後)追加・削除・全体の集計をとることができる
template<typename S, S (*op)(S, S), S (*e)()>
struct SWAG {
public:
    int sz;
    vector<S> dat_l, dat_r;
    vector<S> cum_l, cum_r;
    
    SWAG(): sz(0), cum_l({e()}), cum_r({e()}) {}

    int size() {return sz;}

    void push_front(S x) {
        ++sz;
        dat_l.push_back(x);
        cum_l.push_back(op(x, cum_l.back()));
    }

    void push_back(S x) {
        ++sz;
        dat_r.push_back(x);
        cum_r.push_back(op(cum_r.back(), x));
    }

    void clear() {
        sz = 0;
        dat_l.clear(), dat_r.clear();
        cum_l = {e()}, cum_r = {e()};
    }

    void pop_front() {
        if(sz == 0) {
            cerr << "Assertion Failed\n";
            cerr << "`pop_fron` operation was denied because the size of queue is 0\n";
            return;
        }
        if(sz == 1) return clear();
        if(dat_l.empty()) shifting_adjust();
        --sz;
        dat_l.pop_back();
        cum_l.pop_back();
    }

    void pop_back() {
        if(sz == 0) {
            cerr << "Assertion Failed\n";
            cerr << "`pop_fron` operation was denied because the size of queue is 0\n";
            return;
        }
        if(sz == 1) return clear();
        if(dat_r.empty()) shifting_adjust();
        --sz;
        dat_r.pop_back();
        cum_r.pop_back();
    }

    void shifting_adjust() {
        vector<S> x;
        for(int i = (int)dat_l.size()-1; i >= 0; --i) x.push_back(dat_l[i]);
        x.insert(x.end(), dat_r.begin(), dat_r.end());
        // for(int i = 0; i < (int)dat_r.size(); i++) x.push_back(dat_r);
        clear();
        int m = (int)x.size() / 2;
        
        for(int i = m-1; i >= 0; --i) push_front(x[i]);
        for(int i = m; i < (int)x.size(); i++) push_back(x[i]);
        assert(sz == (int)x.size());
    }

    S prod_all() {
        return op(lprod(), rprod());
    }
private:
    S lprod() {return cum_l.back();}
    S rprod() {return cum_r.back();}
};

template<typename S, S (*op)(S, S), S (*e)()>
vector<S> SWAG_Range_Query(vector<S> val, vector<pair<int, int>> segs) {
    //not implemented
    return {};   
}

//opは非可換でOK (Affineなど)
//全てのクエリについて均し計算量O(1)(shifting_adjustが重そうに見えるが実は合計でO(Q)(Q:クエリ回数)以下しかかからない)