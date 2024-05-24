int euler_phi(int n) {
    int res = n;
    for(int i = 2; i * i <= n; i++) {
        if(n % i != 0) continue;
        res -= res / i;
        while(n%i == 0) n /= i;
    }
    if(n > 1) res -= res / n;
    return res;
}

//calculate φ(i) for all 1<=i<=n
vector<int> euler_phi_table(int n) {
    vector<int> euler(n + 1);
    for(int i = 0; i <= n; i++) {
        euler[i] = i;
    }
    for(int i = 2; i <= n; i++) {
        if(euler[i] == i) {
            for(int j = i; j <= n; j += i) {
                euler[j] = euler[j] / i * (i - 1);
            }
        }
    }
    return euler;
}