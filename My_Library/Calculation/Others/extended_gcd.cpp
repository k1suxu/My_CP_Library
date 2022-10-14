//Calculation
//Others
//extended_gcd
//
// return : gcd(a, b)
// ax + by = gcd(a, b) を満たす(x, y)の格納
int extended_gcd(int a, int b, int &x, int &y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll d = extended_gcd(b, a%b, y, x);
    y -= a/b*x;
    return d;
}