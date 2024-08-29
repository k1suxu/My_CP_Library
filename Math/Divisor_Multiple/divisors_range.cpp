// divs[i] = iの約数の一覧 O(n log n)
vector<vector<long long>> divisors(const long long n) {
    vector<vector<long long>> ret(n);
    repi(i, 1, n) for(int j = i; j < n; j += i) ret[j].push_back(i);
    return ret;
}