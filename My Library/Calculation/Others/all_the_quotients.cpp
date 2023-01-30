vector<int> quotients(const int n) {
    vector<int> ret;

    for(int i = 1; i * i <= n; i++) {
        if(n%i != 0) continue;
        ret.push_back(i);
        if(i * i  != n) ret.push_back(n / i);
    }

    sort(all(ret));
    return ret;
}