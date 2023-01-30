//Calculation
//Fraction Struct
//fraction

// x / y
struct fraction {

    int x, y;

    // a / b (Êó¢Á¥?Âà?Êï∞„Å´„Å™„Ç?)
    fraction(int a = 0, int b = 1) {
        int divisor = __gcd(a, b);
        x = a / divisor;
        y = b / divisor;
    }

    long double get() const {
        return (long double)x / (long double)y;
    }


    fraction operator+(const fraction &other) const {
        int divisor = __gcd(x * other.y + y * other.x, y * other.y);
        return {(x * other.y + y * other.x) / divisor, y * other.y / divisor};
    }
    fraction operator-(const fraction &other) const {
        int divisor = __gcd(x * other.y - y * other.x, y * other.y);
        return {(x * other.y - y * other.x) / divisor, y * other.y / divisor};
    }
    fraction operator*(const fraction &other) const {
        int ret_x = x * other.x;
        int ret_y = y * other.y;
        int divisor = __gcd(ret_x, ret_y);
        return {ret_x / divisor, ret_y / divisor};
    }
    fraction operator/(const fraction &other) const {
        int ret_x = x * other.y;
        int ret_y = y * other.x;
        int divisor = __gcd(ret_x, ret_y);
        return {ret_x / divisor, ret_y / divisor};
    }

    fraction& operator+=(const fraction &other) {
        *this = *this + other;
        return *this;
    }
    fraction& operator-=(const fraction &other) {
        *this = *this - other;
        return *this;
    }
    fraction& operator*=(const fraction &other) {
        *this = *this * other;
        return *this;
    }
    fraction& operator/=(const fraction &other) {
        *this = *this / other;
        return *this;
    }

    bool operator==(const fraction &other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const fraction &other) const {
        return x != other.x || y != other.y;
    }
    bool operator<(const fraction &other) const {
        if(y * other.y > 0) return x * other.y < y * other.x;
        return x * other.y > y * other.x;
    }
    bool operator>(const fraction &other) const {
        if(y * other.y > 0) return x * other.y > y * other.x;
        return x * other.y < y * other.x;
    }
    bool operator<=(const fraction &other) const {
        if(y * other.y > 0) return x * other.y <= y * other.x;
        return x * other.y >= y * other.x;
    }
    bool operator>=(const fraction &other) const {
        if(y * other.y > 0) return x * other.y >= y * other.x;
        return x * other.y <= y * other.x;
    }
    fraction operator+(const int &other) const {return (*this) + fraction(other);}
    fraction operator-(const int &other) const {return (*this) - fraction(other);}
    fraction operator*(const int &other) const {return (*this) * fraction(other);}
    fraction operator/(const int &other) const {return (*this) / fraction(other);}
    fraction operator+=(const int &other) {(*this) += fraction(other); return *this;}
    fraction operator-=(const int &other) {(*this) -= fraction(other); return *this;}
    fraction operator*=(const int &other) {(*this) *= fraction(other); return *this;}
    fraction operator/=(const int &other) {(*this) /= fraction(other); return *this;}
    bool operator==(const int &other) const {return (*this) == fraction(other);}
    bool operator!=(const int &other) const {return (*this) != fraction(other);}
    bool operator<(const int &other) const {return (*this) < fraction(other);}
    bool operator>(const int &other) const {return (*this) > fraction(other);}
    bool operator<=(const int &other) const {return (*this) <= fraction(other);}
    bool operator>=(const int &other) const {return (*this) >= fraction(other);}
};