// define int long longは使えないことに注意
// ref: https://xuzijian629.hatenablog.com/entry/2018/12/01/000010
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template<typename T, template<typename> class comp>
using pbds_tree = tree<T, null_type, comp<T>, rb_tree_tag, tree_order_statistics_node_update>;

// // count the number of `x` such that x \in [l, r).
// template<typename T, template<typename> class comp>
// int range_count(const pbds_tree<T, comp> &tree, T l, T r) {
//     if(l >= r) return 0;
//     return tree.order_of_key(r) - tree.order_of_key(l);
// }

// // pbds_tree<int, less> [[VAR_NAMWE]] のように使う