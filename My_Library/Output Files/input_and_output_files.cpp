template<typename T>
istream &operator>>(istream &is, vector<T> &vec) {
    int n = (int)vec.size();
    for(int i = 0; i < n; i++) is >> vec[i];
    return is;
};
template<typename T>
ostream &operator<<(ostream &os, vector<T> &vec) {
    int n = (int)vec.size();
    for(int i = 0; i < n; i++) {
        if(i != n - 1) os << vec[i] << " ";
        else os << vec[i] << "\n";
    }
    return os;
}

template<typename T, typename U>
istream &operator>>(istream &os, pair<T, U> &p) {
    T a;
    U b;
    os >> a >> b;
    p = make_pair(a, b);
    return os;
}
template<typename T, typename U>
ostream &operator<<(ostream &os, pair<T, U> &p) {
    os << p.first << " " << p.second;
    return os;
}