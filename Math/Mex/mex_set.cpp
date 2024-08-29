// ref: https://drken1215.hatenablog.com/entry/2023/11/26/233800
// removable min heap
template<class T> struct removable_min_heap {
    // inner data
    priority_queue<T, vector<T>, greater<T>> que, delay;
    
    // constructor
    removable_min_heap() {}

    // push(x), remove(x)
    void push(T x) { que.push(x); }
    void remove(T x) { delay.push(x); }
    int size() { return (int)que.size() - (int)delay.size(); }
    
    // pop min value
    T pop() {
        T res = top();
        que.pop();
        return res;
    }
    
    // get min value (not pop)
    T top() {
        assert(!que.empty());
        while (!delay.empty() && que.top() == delay.top()) {
            que.pop();
            delay.pop();
        }
        assert(!que.empty());
        return que.top();
    }
};

struct mex_multiset {
    map<int, int> mp;
    removable_min_heap<int> que;

    mex_multiset(int max_mex) {
        rep(i, max_mex+1) que.push(i);
    }

    void del(int x) {
        --mp[x];
        if(mp[x] == 0) que.push(x);
    }

    void insert(int x) {
        if(mp[x] == 0) que.remove(x);
        ++mp[x];
    }

    int get_mex() {
        return que.top();
    }
};

int find_mex(vector<int> x) {
    mex_multiset mex(x.size());
    return mex.get_mex();
}