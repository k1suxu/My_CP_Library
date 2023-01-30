int lcm(int a, int b) {
    return a / __gcd(a, b) * b;
}

int lcm_arr(vector<int> v) {
    int l = 1;
    for(auto e : v) {
        l = lcm(l, e);
    }
    return l;
}