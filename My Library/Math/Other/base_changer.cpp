//10->base
string to_any_base(long long base, long long n) {
    assert(base <= 36);
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
    assert(base <= 36);
    long long ret = 0;
    long long mul = 1;
    for(int i = (int)n.size() - 1; i >= 0; i--) {
        if('A' <= n[i] && n[i] <= 'Z') ret += (n[i] - 'A' + 10) * mul;
        else ret += (n[i] - '0') * mul;
        mul *= base;
    }
    return ret;
}

string to_any_base_decimal(long long to_base, long double decimal) {
    assert(to_base <= 36);
    assert(decimal < 1);
    string ans;
    while(decimal != (long double)0) {
        decimal *= to_base;
        if(decimal >= 1) {
            if((int)decimal <= 9) ans.push_back((char)('0' + (int)decimal));
            else ans.push_back((char)('A' + (int)decimal - 10));
            decimal -= (int)decimal;
        }else {
            ans.push_back('0');
        }
    }
    return ans;
}

long double from_any_base_decimal(long long base, string n) {
    assert(base <= 36);
    long double ans = 0;
    long double fac = 1;
    for(const auto &c : n) {
        fac /= base;
        if('A' <= c && c <= 'Z') ans += (int)(c-'A'+10) * fac;
        else ans += (int)(c - '0') * fac;
    }
    return ans;
}

string from_any_to_any_base(long long from_base, long long to_base, string n) {
    return to_any_base(to_base, from_any_base(from_base, n));
}
string from_any_to_any_base_decimal(long long from_base, long long to_base, string n) {
    return to_any_base_decimal(to_base, from_any_base_decimal(from_base, n));
}