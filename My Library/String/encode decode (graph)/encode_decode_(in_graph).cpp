//これは三文字の場合
int encode(const string &s) {
    int res = 0;
    for(auto c : s) res = res * 3 + (c - 'A');
    return res;
}

string decode(int n, int v) {
    string res = "";
    while(v) {
        res += (char)('A' + v%3);
        v /= 3;
    }
    while(res.size() < n) res += 'A';
    reverse(res.begin(), res.end());
    return res;
}