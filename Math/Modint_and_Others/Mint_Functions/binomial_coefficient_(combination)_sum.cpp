Mint inv2 = Mint(2).inv();

Mint sum = 1;
ll n = 0, k = 1;
auto getcombsum = [&](ll nn, ll kk) -> Mint {
    if (kk <= 0) return 0;
    // nadd
    while (n < nn) {
        n++;
        sum = sum * 2 - comb(n - 1, k - 1);
    }
    // nsub
    while (n > nn) {
        sum = (sum + comb(n - 1, k - 1)) * inv2;
        n--;
    }
    // kadd
    while (k < kk) {
        // sum += comb(n, k++);
        sum += comb(n, k);
        k++;
    }
    while (k > kk) {
        // sum -= comb(n, --k);
        --k;
        sum -= comb(n, k);
    }
    return sum;
};