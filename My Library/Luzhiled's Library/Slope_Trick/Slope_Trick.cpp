// ref: https://ei1333.github.io/library/structure/others/slope-trick.hpp
template< typename T >
struct SlopeTrick {
    
    const T INF = numeric_limits< T >::max() / 3;
    
    T min_f;
    priority_queue< T, vector< T >, less<> > L;
    priority_queue< T, vector< T >, greater<> > R;
    T add_l, add_r;
    
    private:
    void push_R(const T &a) {
        R.push(a - add_r);
    }
    
    T top_R() const {
        if(R.empty()) return INF;
        else return R.top() + add_r;
    }
    
    T pop_R() {
        T val = top_R();
        if(not R.empty()) R.pop();
        return val;
    }
    
    void push_L(const T &a) {
        L.push(a - add_l);
    }
    
    T top_L() const {
        if(L.empty()) return -INF;
        else return L.top() + add_l;
    }
    
    T pop_L() {
        T val = top_L();
        if(not L.empty()) L.pop();
        return val;
    }
    
    size_t size() {
        return L.size() + R.size();
    }
 
public:
    SlopeTrick() : min_f(0), add_l(0), add_r(0) {}
    
    //min x, max x, min f(x)
    array<T, 3> query() const {
        return {top_L(), top_R(), min_f};
    }
    
    // f(x) += a
    void add_all(const T &a) {
        min_f += a;
    }
    
    // add \_
    // f(x) += max(a - x, 0)
    void add_a_minus_x(const T &a) {
        min_f += max(T(0), a - top_R());
        push_R(a);
        push_L(pop_R());
    }
    
    // add _/
    // f(x) += max(x - a, 0)
    void add_x_minus_a(const T &a) {
        min_f += max(T(0), top_L() - a);
        push_L(a);
        push_R(pop_L());
    }
    
    // add \/
    // f(x) += abs(x - a)
    void add_abs(const T &a) {
        add_a_minus_x(a);
        add_x_minus_a(a);
    }
    
    // \/ -> \_
    // f_{new} (x) = min f(y) (y <= x)
    void clear_right() {
        while(not R.empty()) R.pop();
    }
    
    // \/ -> _/
    // f_{new} (x) = min f(y) (y >= x)
    void clear_left() {
        while(not L.empty()) L.pop();
    }
    
    // \/ -> \_/
    // f_{new} (x) = min f(y) (x-b <= y <= x-a)
    void shift(const T &a, const T &b) {
        assert(a <= b);
        add_l += a;
        add_r += b;
    }
    
    // \/. -> .\/
    // f_{new} (x) = f(x - a)
    void shift(const T &a) {
        shift(a, a);
    }
    
    // L, R を破壊する
    T get(const T &x, bool retain = false) {
        SlopeTrick<T> before;
        if(retain) {
            before = (*this);
        }
        T ret = min_f;
        while(not L.empty()) {
            ret += max(T(0), pop_L() - x);
        }
        while(not R.empty()) {
            ret += max(T(0), x - pop_R());
        }
        if(retain) (*this) = before;
        return ret;
    }
    
    void merge(SlopeTrick<T> &g, bool retain = false) {
        SlopeTrick<T> before;
        if(retain) {
            before = g;
        }
        if(g.size() > size()) {
            swap(g.L, L);
            swap(g.R, R);
            swap(g.add_l, add_l);
            swap(g.add_r, add_r);
            swap(g.min_f, min_f);
        }
        while(not g.R.empty()) {
            add_x_minus_a(g.pop_R());
        }
        while(not g.L.empty()) {
            add_a_minus_x(g.pop_L());
        }
        min_f += g.min_f;
        if(retain) g = before;
    }
};

//explanation
/*
query() : min(f(x))とその時のxの最小値最大値を返す
add_all(a) : f(x) = f(x) + a
add_a_minus_x(a) : f(x) = f(x) + max(a-x, 0) -> 右肩下がりから直線
add_x_minus_a(a) : f(x) = f(x) + max(x-a, 0) -> 直線から右肩上がり
add_abs(a) : f(x) = f(x) + |x-a| = f(x) + max(a-x, 0) + max(x-a, 0)
clear_right() : f(x) = 
clear_left() : 
shift(a, b) : f(x)=min_{x-b<=y<=x-a}f(y) (a<=bが必須)
shift(a) : f(x) = f(x-a)
get(x) : f(x)を返す(fは破壊される)
merge(g(x)) : f(x) = f(x) + g(x) (gは破壊される)
*/