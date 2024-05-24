// cf: https://snuke.hatenablog.com/entry/2014/12/02/235837
// 返ってくる値は各iを中心とした回文の最大長さ 半径は(Rad=(len[i]+1)/2)で求められる
// 原則奇数長しか返せない
// dummy文字を入れることで偶数長も求められる -> (ABBA -> A$B$B$A)
// 結局dummmy文字はdummy文字としか比較されないため、str[0]をdumyとして採用してもよい
// calc_even=trueの場合、奇偶奇偶...という配列が返ってくる
template<typename T_String>
vector<int> manacher(T_String str, bool calc_even = false) {
    if(calc_even) {
        int n = (int)str.size();
        assert(n > 0);
        str.resize(2*n-1);
        for(int i = n-1; i >= 0; i--) {
            str[2*i] = str[i];
        }
        for(int i = 0; i < n-1; i++) {
            str[2*i+1] = str[0];
        }
    }

    int n = (int)str.size();
    vector<int> len(n);

    {
        int i = 0, j = 0;
        while(i < n) {
            while(i-j >= 0 && i+j < n && str[i-j] == str[i+j]) ++j;
            len[i] = j;
            int k = 1;
            while(i-k >- 0 && i+k < n && k+len[i-k] < j) {
                len[i+k] = len[i-k];
                ++k;
            }
            i += k, j -= k;
        }
    }

    if(calc_even) {
        for(int i = 0; i < n; i++) {
            if(((i ^ len[i]) & 1) == 0) --len[i];
        }
    }else {
        for(auto& x : len) x = 2 * x -1;
    }
    return len;
}

/* [回文判定] [閉区間]
auto is_palindrome = [&](int l, int r) -> bool {
    if(r-l+1==1) return true;
    int center = (l + r);
    return r-l+1 <= mana[center];
};
*/