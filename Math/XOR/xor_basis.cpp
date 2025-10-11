template<typename T>
vector<T> unique_xor_basis(vector<T> vec) {
    vector<T> basis;
    for (auto e : vec) {
        for (auto b : basis) chmin(e, e ^ b);
        if (e > 0) {
            for (auto &b : basis) chmin(b, e ^ b); // if unique
            basis.emplace_back(e);
        }
    }
    sort(all(basis)); // if need sorting
    return basis;
}