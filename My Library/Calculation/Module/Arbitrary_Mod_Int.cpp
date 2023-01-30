struct Arbitrary_Modular_Int {
    int x;

    Arbitrary_Modular_Int() = default;
    Arbitrary_Modular_Int(int x_) : x(x_ >= 0 ? x_ % get_mod() : (get_mod() - (-x_) % get_mod()) % get_mod()) {}
    
    // static int& get_mod() {
    //     static int mod = 0;
    //     return mod;
    // }
    static int& get_mod() {
        static int mod = 0;
        return mod;
    }
    static void set_mod(int mod_) {
        get_mod() = mod_;
    }

    int val() const {
        return (x%get_mod()+get_mod())%get_mod();
    }
    

    Arbitrary_Modular_Int& operator^=(int d)  {
        Arbitrary_Modular_Int ret(1);
        int nx = x;
        while(d) {
            if(d&1) ret *= nx;
            (nx *= nx) %= get_mod();
            d >>= 1;
        }
        *this = ret;
        return *this;
    }
    Arbitrary_Modular_Int operator^(int d) const {return Arbitrary_Modular_Int(*this) ^= d;}
    Arbitrary_Modular_Int pow(int d) const {return Arbitrary_Modular_Int(*this) ^= d;}
    
    //use this basically
    // Arbitrary_Modular_Int inv() const {
    //     return Arbitrary_Modular_Int(*this) ^ (get_mod()-2);
    // }
    //only if the module number is not prime
    //Don't use. This is broken.
    Arbitrary_Modular_Int inv() const {
        int a = (x%get_mod()+get_mod())%get_mod(), b = get_mod(), u = 1, v = 0;
        while(b > 0) {
            int t = a/b;
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
        int z = x;
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
    
    Arbitrary_Modular_Int& operator+=(const int other) {Arbitrary_Modular_Int other_(other); *this += other_; return *this;}
    Arbitrary_Modular_Int& operator-=(const int other) {Arbitrary_Modular_Int other_(other); *this -= other_; return *this;}
    Arbitrary_Modular_Int& operator*=(const int other) {Arbitrary_Modular_Int other_(other); *this *= other_; return *this;}
    Arbitrary_Modular_Int& operator/=(const int other) {Arbitrary_Modular_Int other_(other); *this /= other_; return *this;}
    Arbitrary_Modular_Int operator+(const int other) const {return Arbitrary_Modular_Int(*this) += other;}
    Arbitrary_Modular_Int operator-(const int other) const {return Arbitrary_Modular_Int(*this) -= other;}
    Arbitrary_Modular_Int operator*(const int other) const {return Arbitrary_Modular_Int(*this) *= other;}
    Arbitrary_Modular_Int operator/(const int other) const {return Arbitrary_Modular_Int(*this) /= other;}

    bool operator==(const Arbitrary_Modular_Int other) const {return (*this).val() == other.val();}
    bool operator!=(const Arbitrary_Modular_Int other) const {return (*this).val() != other.val();}
    bool operator==(const int other) const {return (*this).val() == other;}
    bool operator!=(const int other) const {return (*this).val() != other;}

    Arbitrary_Modular_Int operator-() const {return Arbitrary_Modular_Int(0LL)-Arbitrary_Modular_Int(*this);}
};

istream& operator>>(istream& is, Arbitrary_Modular_Int& x) {
    int X;
    is >> X;
    x = X;
    return is;
}
ostream& operator<<(ostream& os, Arbitrary_Modular_Int& x) {
    os << x.val();
    return os;
}

using mint = Arbitrary_Modular_Int;