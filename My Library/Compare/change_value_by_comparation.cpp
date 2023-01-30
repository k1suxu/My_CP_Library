template<typename T>
bool chmax(T &a, const T &b) {bool flag = false; if(a < b) flag = true; a = (a > b? a : b); return flag;}
template<typename T>
bool chmin(T &a, const T &b) {bool flag = false; if(a > b) flag = true; a = (a < b? a : b); return flag;}