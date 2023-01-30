//Calculation
//Module
//
//n^m%mod
//ba careful 0^0
int powmod(int n, int m) {
    int res = 1;
    while(m > 0) {
        if(m&1) res = (res*n)%mod;
        n = (n*n)%mod;
        m >>= 1;
    }
    return res;
}