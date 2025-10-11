template<long long MOD>
struct Modular_Int {
    using Mint_Type = Modular_Int<MOD>;
    long long x;

    Modular_Int() = default;
    Modular_Int(long long x_) : x(x_ >= 0? x_%MOD : (MOD-(-x_)%MOD)%MOD) {}

    long long val() const {
        return (x%MOD+MOD)%MOD;
    }
    static long long get_mod() {
        return MOD;
    }

    Mint_Type& operator^=(long long d)  {
        Mint_Type ret(1);
        long long nx = x;
        while(d) {
            if(d&1) ret *= nx;
            (nx *= nx) %= MOD;
            d >>= 1;
        }
        *this = ret;
        return *this;
    }
    Mint_Type operator^(long long d) const {return Mint_Type(*this) ^= d;}
    Mint_Type pow(long long d) const {return Mint_Type(*this) ^= d;}
    
    //use this basically
    Mint_Type inv() const {
        assert(val() != 0);
        return Mint_Type(*this) ^ (MOD-2);
    }
    // only if the module number is not prime
    // Mint_Type inv() const {
    //     long long a = (x%MOD+MOD)%MOD, b = MOD, u = 1, v = 0;
    //     while(b) {
    //         long long t = a/b;
    //         a -= t*b, swap(a, b);
    //         u -= t*v, swap(u, v);
    //     }
    //     return Mint_Type(u);
    // }

    Mint_Type& operator+=(const Mint_Type other) {
        if((x += other.x) >= MOD) x -= MOD;
        return *this;
    }
    Mint_Type& operator-=(const Mint_Type other) {
        if((x -= other.x) < 0) x += MOD;
        return *this;
    }
    Mint_Type& operator*=(const Mint_Type other) {
        long long z = x;
        z *= other.x;
        z %= MOD;
        x = z;
        if(x < 0) x += MOD;
        return *this;
    }
    Mint_Type& operator/=(const Mint_Type other) {
        return *this = *this * other.inv();
    }
    Mint_Type& operator++() {
        x++;
        if (x == MOD) x = 0;
        return *this;
    }
    Mint_Type& operator--() {
        if (x == 0) x = MOD;
        x--;
        return *this;
    }
    
    Mint_Type operator+(const Mint_Type other) const {return Mint_Type(*this) += other;}
    Mint_Type operator-(const Mint_Type other) const {return Mint_Type(*this) -= other;}
    Mint_Type operator*(const Mint_Type other) const {return Mint_Type(*this) *= other;}
    Mint_Type operator/(const Mint_Type other) const {return Mint_Type(*this) /= other;}
    
    Mint_Type& operator+=(const long long other) {Mint_Type other_(other); *this += other_; return *this;}
    Mint_Type& operator-=(const long long other) {Mint_Type other_(other); *this -= other_; return *this;}
    Mint_Type& operator*=(const long long other) {Mint_Type other_(other); *this *= other_; return *this;}
    Mint_Type& operator/=(const long long other) {Mint_Type other_(other); *this /= other_; return *this;}
    Mint_Type operator+(const long long other) const {return Mint_Type(*this) += other;}
    Mint_Type operator-(const long long other) const {return Mint_Type(*this) -= other;}
    Mint_Type operator*(const long long other) const {return Mint_Type(*this) *= other;}
    Mint_Type operator/(const long long other) const {return Mint_Type(*this) /= other;}

    bool operator==(const Mint_Type other) const {return (*this).val() == other.val();}
    bool operator!=(const Mint_Type other) const {return (*this).val() != other.val();}
    bool operator==(const long long other) const {return (*this).val() == other;}
    bool operator!=(const long long other) const {return (*this).val() != other;}

    Mint_Type operator-() const {return Mint_Type(0LL)-Mint_Type(*this);}

    //-1: sqrtが存在しない
    //複数存在する場合どれを返すかは不明
    long long get_sqrt() const {
        long long a = val(), p = get_mod();
        if(a == 0) return 0;
        if(p == 2) return a;
        if(Mint_Type(a).pow((p - 1) >> 1).val() != 1) return -1;
        long long b = 1;
        while(Mint_Type(b).pow((p - 1) >> 1).val() == 1) ++b;
        long long e = 0, m = p - 1;
        while(m % 2 == 0) m >>= 1, ++e;
        long long x = Mint_Type(a).pow((m - 1) >> 1).val();
        long long y = a * (x * x % p) % p;
        (x *= a) %= p;
        long long z = Mint_Type(b).pow(m).val();
        while(y != 1) {
            long long j = 0, t = y;
            while(t != 1) {
                j += 1;
                (t *= t) %= p;
            }
            z = Mint_Type(z).pow((long long)1 << (e - j - 1)).val();
            (x *= z) %= p;
            (z *= z) %= p;
            (y *= z) %= p;
            e = j;
        }
        return x;
    }

    template <typename T>
    friend Mint_Type operator+(T t, const Mint_Type& o) {
        return o + t;
    }
    template <typename T>
    friend Mint_Type operator-(T t, const Mint_Type& o) {
        return -o + t;
    }
    template <typename T>
    friend Mint_Type operator*(T t, const Mint_Type& o) {
        return o * t;
    }
    template <typename T>
    friend Mint_Type operator/(T t, const Mint_Type& o) {
        return o.inv() * t;
    }
};

// TODO: SELECT MOD_VAL
// const long long MOD_VAL = 1e9+7;
const long long MOD_VAL = 998244353;
using mint = Modular_Int<MOD_VAL>;

istream& operator>>(istream& is, mint& x) {
    long long X;
    is >> X;
    x = X;
    return is;
}
ostream& operator<<(ostream& os, mint& x) {
    os << x.val();
    return os;
}

// 1e9 + 7をmodとして使いたいときに注意！！！！特にCFやCCなどのAtCoder以外