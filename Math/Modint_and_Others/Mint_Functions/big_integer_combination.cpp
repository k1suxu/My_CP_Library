// nCkをO(k)で求める
mint big_binom(long long n, long long k) {
    if(n<k) {
        cerr << "\nAssertion Failed!!\n";
        cerr << "Expression N >= K, ";
        cerr << "where n=" << n << ", k=" << k << "\n\n";
        return 0;
    }
    mint ans = 1;
    for(long long i = 1; i <= k; i++) {
        ans *= n-i+1;
        ans /= i;
    }
    return ans;
}
// O(min(n, k))
mint big_h_binom(long long n, long long k) {
    assert(n >= 1);
    return big_binom(n+k-1, min(n-1, k));
}