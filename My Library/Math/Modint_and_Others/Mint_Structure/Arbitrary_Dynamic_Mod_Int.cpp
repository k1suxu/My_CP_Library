struct Arbitrary_Modular_Int {
    long long x;

    Arbitrary_Modular_Int() = default;
    Arbitrary_Modular_Int(long long x_) : x(x_ >= 0 ? x_ % get_mod() : (get_mod() - (-x_) % get_mod()) % get_mod()) {}
    
    // static long long& get_mod() {
    //     static long long mod = 0;
    //     return mod;
    // }
    static long long& get_mod() {
        static long long mod = 0;
        return mod;
    }
    static void set_mod(long long mod_) {
        get_mod() = mod_;
    }

    long long val() const {
        return (x%get_mod()+get_mod())%get_mod();
    }
    

    Arbitrary_Modular_Int& operator^=(long long d)  {
        Arbitrary_Modular_Int ret(1);
        long long nx = x;
        while(d) {
            if(d&1) ret *= nx;
            (nx *= nx) %= get_mod();
            d >>= 1;
        }
        *this = ret;
        return *this;
    }
    Arbitrary_Modular_Int operator^(long long d) const {return Arbitrary_Modular_Int(*this) ^= d;}
    Arbitrary_Modular_Int pow(long long d) const {return Arbitrary_Modular_Int(*this) ^= d;}
    
    //use this basically
    // Arbitrary_Modular_Int inv() const {
    //     return Arbitrary_Modular_Int(*this) ^ (get_mod()-2);
    // }
    //only if the module number is not prime
    //Don't use. This is broken.
    Arbitrary_Modular_Int inv() const {
        long long a = (x%get_mod()+get_mod())%get_mod(), b = get_mod(), u = 1, v = 0;
        while(b > 0) {
            long long t = a/b;
            a -= t*b, swap(a, b);
            u -= t*v, swap(u, v);
        }
        return Arbitrary_Modular_Int(u);
    }

    Arbitrary_Modular_Int& operator+=(const Arbitrary_Modular_Int other) {
        if((x += other.x) >= get_mod()) x -= get_mod();
        return *this;
    }
    Arbitrary_Modular_Int& operator-=(const Arbitrary_Modular_Int other) {
        if((x -= other.x) < 0) x += get_mod();
        return *this;
    }
    Arbitrary_Modular_Int& operator*=(const Arbitrary_Modular_Int other) {
        long long z = x;
        z *= other.x;
        z %= get_mod();
        x = z;
        if(x < 0) x += get_mod();
        return *this;
    }
    Arbitrary_Modular_Int& operator/=(const Arbitrary_Modular_Int other) {
        return *this = *this * other.inv();
    }
    Arbitrary_Modular_Int& operator++() {
        x++;
        if (x == get_mod()) x = 0;
        return *this;
    }
    Arbitrary_Modular_Int& operator--() {
        if (x == 0) x = get_mod();
        x--;
        return *this;
    }
    
    Arbitrary_Modular_Int operator+(const Arbitrary_Modular_Int other) const {return Arbitrary_Modular_Int(*this) += other;}
    Arbitrary_Modular_Int operator-(const Arbitrary_Modular_Int other) const {return Arbitrary_Modular_Int(*this) -= other;}
    Arbitrary_Modular_Int operator*(const Arbitrary_Modular_Int other) const {return Arbitrary_Modular_Int(*this) *= other;}
    Arbitrary_Modular_Int operator/(const Arbitrary_Modular_Int other) const {return Arbitrary_Modular_Int(*this) /= other;}
    
    Arbitrary_Modular_Int& operator+=(const long long other) {Arbitrary_Modular_Int other_(other); *this += other_; return *this;}
    Arbitrary_Modular_Int& operator-=(const long long other) {Arbitrary_Modular_Int other_(other); *this -= other_; return *this;}
    Arbitrary_Modular_Int& operator*=(const long long other) {Arbitrary_Modular_Int other_(other); *this *= other_; return *this;}
    Arbitrary_Modular_Int& operator/=(const long long other) {Arbitrary_Modular_Int other_(other); *this /= other_; return *this;}
    Arbitrary_Modular_Int operator+(const long long other) const {return Arbitrary_Modular_Int(*this) += other;}
    Arbitrary_Modular_Int operator-(const long long other) const {return Arbitrary_Modular_Int(*this) -= other;}
    Arbitrary_Modular_Int operator*(const long long other) const {return Arbitrary_Modular_Int(*this) *= other;}
    Arbitrary_Modular_Int operator/(const long long other) const {return Arbitrary_Modular_Int(*this) /= other;}

    bool operator==(const Arbitrary_Modular_Int other) const {return (*this).val() == other.val();}
    bool operator!=(const Arbitrary_Modular_Int other) const {return (*this).val() != other.val();}
    bool operator==(const long long other) const {return (*this).val() == other;}
    bool operator!=(const long long other) const {return (*this).val() != other;}

    Arbitrary_Modular_Int operator-() const {return Arbitrary_Modular_Int(0LL)-Arbitrary_Modular_Int(*this);}

    //-1: sqrtが存在しない
    //複数存在する場合どれを返すかは不明
    long long get_sqrt() {
        long long a = val(), p = get_mod();
        if(a == 0) return 0;
        if(p == 2) return a;
        if(Arbitrary_Modular_Int(a).pow((p - 1) >> 1).val() != 1) return -1;
        long long b = 1;
        while(Arbitrary_Modular_Int(b).pow((p - 1) >> 1).val() == 1) ++b;
        long long e = 0, m = p - 1;
        while(m % 2 == 0) m >>= 1, ++e;
        long long x = Arbitrary_Modular_Int(a).pow((m - 1) >> 1).val();
        long long y = a * (x * x % p) % p;
        (x *= a) %= p;
        long long z = Arbitrary_Modular_Int(b).pow(m).val();
        while(y != 1) {
            long long j = 0, t = y;
            while(t != 1) {
                j += 1;
                (t *= t) %= p;
            }
            z = Arbitrary_Modular_Int(z).pow((long long)1 << (e - j - 1)).val();
            (x *= z) %= p;
            (z *= z) %= p;
            (y *= z) %= p;
            e = j;
        }
        return x;
    }
};

istream& operator>>(istream& is, Arbitrary_Modular_Int& x) {
    long long X;
    is >> X;
    x = X;
    return is;
}
ostream& operator<<(ostream& os, Arbitrary_Modular_Int& x) {
    os << x.val();
    return os;
}

using mint = Arbitrary_Modular_Int;