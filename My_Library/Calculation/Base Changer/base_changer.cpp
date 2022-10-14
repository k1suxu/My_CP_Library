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