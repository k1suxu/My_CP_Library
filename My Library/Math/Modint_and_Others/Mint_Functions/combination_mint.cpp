vector<mint> fact = {1}, fact_inv = {1};
void factor_init(long long n) {
    ++n;
    fact.resize(n, 0);
    fact_inv.resize(n, 0);
    fact[0] = 1;
    repi(i, 1, n) fact[i] = (fact[i - 1] * i);
    fact_inv[n-1] = fact[n-1].inv();
    for(int i = n-1; i > 0; --i) fact_inv[i-1] = fact_inv[i] * i;
}
mint P(long long n, long long k) {
    if(n<k) {
        cerr << "\nAssertion Failed!!\n";
        cerr << "Expression N >= K, ";
        cerr << "where n=" << n << ",k=" << k << "\n\n";
        return 0;
    }
    while(n > fact.size()-1) {
        fact.push_back(fact.back() * fact.size());
        fact_inv.push_back(fact.back().inv());
    }
    return fact[n] * fact_inv[n-k];
}
mint C(long long n, long long k) {
    if(n<k) {
        cerr << "\nAssertion Failed!!\n";
        cerr << "Expression N >= K, ";
        cerr << "where n=" << n << ",k=" << k << "\n\n";
        return 0;
    }
    while(n > fact.size()-1) {
        fact.push_back(fact.back() * fact.size());
        fact_inv.push_back(fact.back().inv());
    }
    return fact[n]*fact_inv[n-k]*fact_inv[k];
}
mint H(long long n, long long k) {
    assert(n>=1);
    return C(n+k-1, k);
}
mint Cat(long long n) {
    return C(2*n, n)-C(2*n, n-1);
}