struct point_min_range_min{
    template<typename T>
    static T id(){
        return std::numeric_limits<T>::max();
    }
    template<typename T>
    static T update(T a, T b){
        return std::min(a, b);
    }
    template<typename T>
    static T merge(T a, T b){
        return std::min(a, b);
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_min_range_second_min{
    template<typename T>
    static T id(){
        return {std::numeric_limits<long long>::max(), std::numeric_limits<long long>::max()};
    }
    template<typename T>
    static T update(T a, T b){
        if(a.first <= b.first) return {a.first, std::min(a.second, b.first)};
        else return {b.first, std::min(a.first, b.second)};
    }
    template<typename T>
    static T merge(T a, T b){
        if(a.first <= b.first) return {a.first, std::min(a.second, b.first)};
        else return {b.first, std::min(a.first, b.second)};
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_max_range_max{
    template<typename T>
    static T id(){
        return std::numeric_limits<T>::min();
    }
    template<typename T>
    static T update(T a, T b){
        return std::max(a, b);
    }
    template<typename T>
    static T merge(T a, T b){
        return std::max(a, b);
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_max_range_second_max{
    template<typename T>
    static T id(){
        return {std::numeric_limits<long long>::min(), std::numeric_limits<long long>::min()};
    }
    template<typename T>
    static T update(T a, T b){
        if(a.first >= b.first) return {a.first, std::min(a.second, b.first)};
        else return {b.first, std::min(a.first, b.second)};
    }
    template<typename T>
    static T merge(T a, T b){
        if(a.first >= b.first) return {a.first, std::min(a.second, b.first)};
        else return {b.first, std::min(a.first, b.second)};
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_mul_range_mul{
    template<typename T>
    static T id(){
        return 1;
    }
    template<typename T>
    static T update(T a, T b){
        return a * b;
    }
    template<typename T>
    static T merge(T a, T b){
        return a * b;
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_add_range_min{
    template<typename T>
    static T id(){
        return std::numeric_limits<T>::max();
    }
    template<typename T>
    static T update(T a, T b){
        if(a == id<T>()) return b;
        else if(b == id<T>()) return a;
        return a + b;
    }
    template<typename T>
    static T merge(T a, T b){
        return std::min(a, b);
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};

struct point_add_range_max{
    template<typename T>
    static T id(){
        return std::numeric_limits<T>::min();
    }
    template<typename T>
    static T update(T a, T b){
        if(a == id<T>()) return b;
        else if(b == id<T>()) return a;
        return a + b;
    }
    template<typename T>
    static T merge(T a, T b){
        return std::max(a, b);
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};

struct point_add_range_sum{
    template<typename T>
    static T id(){
        return 0;
    }
    template<typename T>
    static T update(T a, T b){
        return a + b;
    }
    template<typename T>
    static T merge(T a, T b){
        return a + b;
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_set_range_composite{
    template<typename T>
    static T id(){
        return {1, 0};
    }
    template<typename T>
    static T update(T a, T b){
        return b;
    }
    template<typename T>
    static T merge(T a, T b){
        mint xy = a.first * b.first;
        mint ab = a.second * b.first + b.second;
        return {xy, ab};
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct point_set_range_composite_reverse{
    template<typename T>
    static T id(){
        return {1, 0, 0};
    }
    template<typename T>
    static T update(T a, T b){
        return b;
    }
    template<typename T>
    static T reverse(T a){
        return {a[0], a[2], a[1]};
    }
    template<typename T>
    static T merge(T a, T b){
        mint xy = a[0] * b[0];
        mint ab = a[1] * b[0] + b[1];
        mint ba = b[2] * a[0] + a[2];
        return {xy, ab, ba};
    }
};

// 区間set, これまでにsetした物の中ならどれかを取得
// struct range_set_get_any{
//     template<typename Val>
//     static Val id1(){
//         return nullptr;
//     }
//     template<typename Lazy>
//     static Lazy id2(){
//         return nullptr;
//     }
//     template<typename Lazy>
//     static Lazy propagate(Lazy l, Lazy x){
//         return (x == nullptr ? l : x);
//     }
//     template<typename Val, typename Lazy>
//     static Val apply(Val v, Lazy x, int l, int r){
//         return (x == nullptr ? v : x);
//     }
// };

struct range_add_range_sum{
    template<typename T>
    static T id1(){
        return T(0);
    }
    template<typename E>
    static E id2(){
        return E(0);
    }
    template<typename T>
    static T merge(T a, T b){
        return a + b;
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        return a + b * (r - l);
    }
    template<typename E>
    static E propagate(E a, E b){
        return a + b;
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};

struct range_max_range_max{
    template<typename T>
    static T id1(){
        return std::numeric_limits<T>::min();
    }
    template<typename E>
    static E id2(){
        return std::numeric_limits<E>::min();
    }
    template<typename T>
    static T merge(T a, T b){
        return std::max(a, b);
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        return std::max(a, b);
    }
    template<typename E>
    static E propagate(E a, E b){
        return std::max(a, b);
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};
struct range_add_range_max{
    template<typename T>
    static T id1(){
        return std::numeric_limits<T>::min();
    }
    template<typename E>
    static E id2(){
        return 0;
    }
    template<typename T>
    static T merge(T a, T b){
        return std::max(a, b);
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        if(a == id1<T>()) return b * (r - l);
        return a + b;
    }
    template<typename E>
    static E propagate(E a, E b){
        return a + b;
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};

struct range_add_range_min{
    template<typename T>
    static T id1(){
        return std::numeric_limits<T>::max();
    }
    template<typename E>
    static E id2(){
        return 0;
    }
    template<typename T>
    static T merge(T a, T b){
        return std::min(a, b);
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        if(a == id1<T>()) return b * (r - l);
        return a + b;
    }
    template<typename E>
    static E propagate(E a, E b){
        return a + b;
    }
    template<typename T>
    static T reverse(T a){
        return a;
    }
};

struct range_add_range_argmin{
    template<typename T>
    static T id1(){
        return {std::numeric_limits<long long>::max(), -1} ;
    }
    template<typename E>
    static E id2(){
        return 0;
    }
    template<typename T>
    static T merge(T a, T b){
        return std::min(a, b);
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        if(a == id1<T>()) return a;
        return {a.first + b, a.second};
    }
    template<typename E>
    static E propagate(E a, E b){
        return a + b;
    }
    // template<typename T>
    // static T reverse(T a){
    //     return a;
    // }
};

/*
#include <array>
struct range_affine_range_sum{
    template<typename T>
    static T id1(){
        return 0;
    }
    template<typename E>
    static E id2(){
        return E{1, 0};
    }
    template<typename T>
    static T merge(T a, T b){
        return a + b;
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        return a * b[0] + b[1] * (r - l);
    }
    template<typename E>
    static E propagate(E a, E b){
        return E{a[0] * b[0], a[1] * b[0] + b[1]};
    }
};
*/
struct range_affine_range_sum{
    template<typename T>
    static T id1(){
        return 0;
    }
    template<typename E>
    static E id2(){
        return E{1, 0};
    }
    template<typename T>
    static T merge(T a, T b){
        return a + b;
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        return a * b.first + b.second * (r - l);
    }
    template<typename E>
    static E propagate(E a, E b){
        return E{a.first * b.first, a.second * b.first + b.second};
    }
};

struct range_add_range_min_count{
    static constexpr int INF = std::numeric_limits<int>::max();
    template<typename T>
    static T id1(){
        return {INF, 0};
    }
    template<typename E>
    static E id2(){
        return 0;
    }
    template<typename T>
    static T merge(T a, T b){
        if(a.first != b.first) return a.first < b.first ? a : b;
        return {a.first, a.second + b.second};
    }
    template<typename T, typename E>
    static T apply(T a, E b, int l, int r){
        if(a.first == INF) return {b, r - l};
        return {a.first + b, a.second};
    }
    template<typename E>
    static E propagate(E a, E b){
        return a + b;
    }
};

// struct range_composite_lct{
//     template<typename T>
//     // 1x + 0, 1x + 0
//     static T id1(){
//         return std::array<int, 3>{1, 0, 0};
//     }
//     // no use
//     template<typename E>
//     static E id2(){
//         return false;
//     }
//     // b(a(x)), a(b(x))
//     template<typename T>
//     static T merge(T a, T b){
//         int ba1 = b[0] * a[0];
//         int ba2 = b[0] * a[1] + b[1];
//         int ab2 = a[0] * b[2] + a[2];
//         return std::array<int, 3>{ba1, ba2, ab2};
//     }
//     // no use
//     template<typename T, typename E>
//     static T apply(T a, E b, int l, int r){
//         return a;
//     }
//     // no use
//     template<typename E>
//     static E propagate(E a, E b){
//         return false;
//     }
//     //
//     template<typename T>
//     static T reverse(T a){
//         return std::array<int, 3>{a[0], a[2], a[1]};
//     }
// };
