//String
vector<pair<char, int>> run_length_encoding(string s) {
    char need = s[0];
    int cnt = 0;
    vector<pair<char, int>> ret;
    for(char c : s) {
        if(c == need) {
            cnt++;
        }else {
            ret.emplace_back(need, cnt);
            need = c;
            cnt = 1;
        }
    }
    if(cnt != 0) ret.emplace_back(need, cnt);
    return ret;
}

//Others
template<typename T>
vector<pair<T, int>> run_length_encoding(vector<T> s) {
    T need = s[0];
    int cnt = 0;
    vector<pair<T, int>> ret;
    for(T c : s) {
        if(c == need) {
            cnt++;
        }else {
            ret.emplace_back(need, cnt);
            need = c;
            cnt = 1;
        }
    }
    if(cnt != 0) ret.emplace_back(need, cnt);
    return ret;
}