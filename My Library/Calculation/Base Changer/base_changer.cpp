//10->base
string to_any_base(int base, int n) {
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
int from_any_base(int base, string n) {
    int ret = 0;
    int mul = 1;
    for(int i = (int)n.size() - 1; i >= 0; i--) {
        if('A' <= n[i]) ret += (n[i] - 'A' + 10) * mul;
        else ret += (n[i] - '0') * mul;
        mul *= base;
    }
    return ret;
}

string from_any_to_any_base(int from_base, int to_base, string n) {
    return to_any_base(to_base, from_any_base(from_base, n));
}