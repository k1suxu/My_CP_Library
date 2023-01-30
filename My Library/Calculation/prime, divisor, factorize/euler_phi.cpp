//Calculation
//prime, divisor, factorize
//euler_phi
//
//return the number of n(∊ℕ) gcd of which and n is 1
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