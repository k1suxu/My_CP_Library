//10->base
string to_any_base(long long base, long long n) {
    if(n == 0) return "0";
    string ret = "";
    while(n > 0) {
        if(n%base < 10) ret.push_back((char)(n % base + '0'));
        else ret.push_back((char)(n % base - 10 + 'A'));
        n /= base;
    }
    reverse(all(ret));
    return ret;
}

//base->10
long long from_any_base(long long base, string n) {
    long long ret = 0;
    long long mul = 1;
    for(int i = (int)n.size() - 1; i >= 0; i--) {
        if('A' <= n[i]) ret += (n[i] - 'A' + 10) * mul;
        else ret += (n[i] - '0') * mul;
        mul *= base;
    }
    return ret;
}

string from_any_to_any_base(long long from_base, long long to_base, string n) {
    return to_any_base(to_base, from_any_base(from_base, n));
}