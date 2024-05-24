//O(N)
template<typename T_String>
vector<int> Z_algorithm(T_String vec) {
    int n = (int)vec.size();
    vector<int> ret(n, 0);
    int from = -1, last = -1;
    for(int i = 1; i < n; i++) {
        int &len = ret[i];
        if(from != -1) {
            len = min(ret[i - from], last - i);
            len = max((int)0, len);
        }
        while(i + len < n && vec[len] == vec[i + len]) len++;
        if(last < i + len) {
            from = i;
            last = i + len;
        }
    }
    ret[0] = n;
    return ret;
}