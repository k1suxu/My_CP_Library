//Calculation
//Others
//extended_gcd
//
// return : gcd(a, b)
// ax + by = gcd(a, b) を満たす(x, y)の最小値?
long long extended_gcd(long long a, long long b, long long &x, long long &y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long d = extended_gcd(b, a%b, y, x);
    y -= a/b*x;
    return d;
}