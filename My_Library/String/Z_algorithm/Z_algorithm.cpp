//String
//O(N)
vector<int> Z_algorithm(string s) {
    int n = (int)s.size();
    vector<int> ret(n, 0);
    int from = -1, last = -1;
    for(int i = 1; i < n; i++) {
        int &len = ret[i];//何文字一致
        if(from != -1) {
            len = min(ret[i - from], last - i);
            len = max(0LL, len);
        }
        while(i + len < n && s[len] == s[i+len]) len++;
        if(last < i + len) {
            from = i;
            last = i + len;
        }
    }
    ret[0] = n;
    return ret;
}

//Others
template<typename T>
vector<int> Z_algorithm(vector<T> vec) {
    int n = (int)vec.size();
    vector<int> ret(n, 0);
    int from = -1, last = -1;
    for(int i = 1; i < n; i++) {
        int &len = ret[i];
        if(from != -1) {
            len = min(ret[i - from], last - i);
            len = max(0LL, len);
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
