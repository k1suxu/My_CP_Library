template<typename T, typename U>
ostream& operator<<(ostream &os, pair<T, U> p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}
template<typename T>
ostream& operator<<(ostream &os, vector<T> &vec) {
    int size_of_vector = (int)vec.size();
    os << "{";
    for(int i = 0; i < size_of_vector; i++) {
        os << vec[i];
        if(i!=size_of_vector-1) os << ", ";
    }
    os << "}\n";
    return os;
}
template<typename T, size_t N>
ostream& operator<<(ostream &os, array<T, N> ar) {
    os << "{";
    for(int i = 0; i < N; i++) {
        os << ar[i];
        if(i!=N-1) os << ", ";
    }
    os << "}";
    return os;
}

template<typename T>
void dbg(T a) {
    cout << a << endl;
}