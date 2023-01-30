unordered_map<int, int> Inverse_of_Modular_Int_Correspond;
 
template<int MOD>
struct Modular_Int {
    int x;
 
    Modular_Int() = default;
    Modular_Int(int x_) : x(x_ >= 0? x_%MOD : (MOD-(-x_)%MOD)%MOD) {}
 
    int val() const {
        return (x%MOD+MOD)%MOD;
    }
    int get_mod() const {
        return MOD;
    }
 
    Modular_Int<MOD>& operator^=(int d)  {
        Modular_Int<MOD> ret(1);
        int nx = x;
        while(d) {
            if(d&1) ret *= nx;
            (nx *= nx) %= MOD;
            d >>= 1;
        }
        *this = ret;
        return *this;
    }
    Modular_Int<MOD> operator^(int d) const {return Modular_Int<MOD>(*this) ^= d;}
    Modular_Int<MOD> pow(int d) const {return Modular_Int<MOD>(*this) ^= d;}
    
    //use this basically
    Modular_Int<MOD> inv() const {
        static const int x_ = (*this).val();
        if(Inverse_of_Modular_Int_Correspond.find(x) != Inverse_of_Modular_Int_Correspond.end()) return Inverse_of_Modular_Int_Correspond[x];
        static const int ret = (Modular_Int<MOD>(*this) ^ (MOD-2)).val();
        return Inverse_of_Modular_Int_Correspond[x] = ret;
    }
    //only if the module number is not prime
    //Don't use. This is broken.
    // Modular_Int<MOD> inv() const {
    //     int a = (x%MOD+MOD)%MOD, b = MOD, u = 1, v = 0;
    //     while(b) {
    //         int t = a/b;
    //         a -= t*b, swap(a, b);
    //         u -= t*v, swap(u, v);
    //     }
    //     return Modular_Int<MOD>(u);
    // }
 
    Modular_Int<MOD>& operator+=(const Modular_Int<MOD> other) {
        if((x += other.x) >= MOD) x -= MOD;
        return *this;
    }
    Modular_Int<MOD>& operator-=(const Modular_Int<MOD> other) {
        if((x -= other.x) < 0) x += MOD;
        return *this;
    }
    Modular_Int<MOD>& operator*=(const Modular_Int<MOD> other) {
        int z = x;
        z *= other.x;
        z %= MOD;
        x = z;
        if(x < 0) x += MOD;
        return *this;
    }
    Modular_Int<MOD>& operator/=(const Modular_Int<MOD> other) {
        return *this = *this * other.inv();
    }
    Modular_Int<MOD>& operator++() {
        x++;
        if (x == MOD) x = 0;
        return *this;
    }
    Modular_Int<MOD>& operator--() {
        if (x == 0) x = MOD;
        x--;
        return *this;
    }
    
    Modular_Int<MOD> operator+(const Modular_Int<MOD> other) const {return Modular_Int<MOD>(*this) += other;}
    Modular_Int<MOD> operator-(const Modular_Int<MOD> other) const {return Modular_Int<MOD>(*this) -= other;}
    Modular_Int<MOD> operator*(const Modular_Int<MOD> other) const {return Modular_Int<MOD>(*this) *= other;}
    Modular_Int<MOD> operator/(const Modular_Int<MOD> other) const {return Modular_Int<MOD>(*this) /= other;}
    
    Modular_Int<MOD>& operator+=(const int other) {Modular_Int<MOD> other_(other); *this += other_; return *this;}
    Modular_Int<MOD>& operator-=(const int other) {Modular_Int<MOD> other_(other); *this -= other_; return *this;}
    Modular_Int<MOD>& operator*=(const int other) {Modular_Int<MOD> other_(other); *this *= other_; return *this;}
    Modular_Int<MOD>& operator/=(const int other) {Modular_Int<MOD> other_(other); *this /= other_; return *this;}
    Modular_Int<MOD> operator+(const int other) const {return Modular_Int<MOD>(*this) += other;}
    Modular_Int<MOD> operator-(const int other) const {return Modular_Int<MOD>(*this) -= other;}
    Modular_Int<MOD> operator*(const int other) const {return Modular_Int<MOD>(*this) *= other;}
    Modular_Int<MOD> operator/(const int other) const {return Modular_Int<MOD>(*this) /= other;}
 
    bool operator==(const Modular_Int<MOD> other) const {return (*this).val() == other.val();}
    bool operator!=(const Modular_Int<MOD> other) const {return (*this).val() != other.val();}
    bool operator==(const int other) const {return (*this).val() == other;}
    bool operator!=(const int other) const {return (*this).val() != other;}
 
    Modular_Int<MOD> operator-() const {return Modular_Int<MOD>(0LL)-Modular_Int<MOD>(*this);}
 
    //“ü‚êŽq‚É‚µ‚½‚¢
    // friend constexpr istream& operator>>(istream& is, mint& x) noexcept {
    //     int X;
    //     is >> X;
    //     x = X;
    //     return is;
    // }
    // friend constexpr ostream& operator<<(ostream& os, mint& x) {
    //     os << x.val();
    //     return os;
    // }
};
 
// const int MOD_VAL = 1e9+7;
const int MOD_VAL = 998244353;
using mint = Modular_Int<MOD_VAL>;
 
istream& operator>>(istream& is, mint& x) {
    int X;
    is >> X;
    x = X;
    return is;
}
ostream& operator<<(ostream& os, mint& x) {
    os << x.val();
    return os;
}
 
// istream& operator<<(istream& is, mint &a) {
//     int x;
//     is >> x;
//     a = mint(x);
//     return is;
// }
// ostream& operator<<(ostream& os, mint a) {
//     os << a.val();
//     return os;
// }
 
// vector<mint> f = {1}, rf = {1};
// void init(int n) {
//     f.resize(n, 0);
//     rf.resize(n, 0);
//     f[0] = 1;
//     repi(i, 1, n) f[i] = (f[i - 1] * i);
//     repi(i, 0, n) rf[i] = f[i].inv();
// }
// mint P(int n, int k) {
//     assert(n>=k);
//     while(n > f.size()-1) {
//         f.push_back(f.back() * f.size());
//         rf.push_back(f.back().inv());
//     }
//     return f[n] * f[n-k];
// }
// mint C(int n, int k) {
//     assert(n>=k);
//     while(n > f.size()-1) {
//         f.push_back(f.back() * f.size());
//         rf.push_back(f.back().inv());
//     }
//     return f[n]*rf[n-k]*rf[k];
// }
// mint H(int n, int k) {
//     assert(n>=1);
//     return C(n+k-1, k);
// }
// mint Cat(int n) {
//     return C(2*n, n)-C(2*n, n-1);
// }