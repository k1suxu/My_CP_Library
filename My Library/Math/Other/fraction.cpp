struct fraction {

    long long x, y;

    fraction() : x(0), y(1) {}
    fraction(long long x) : x(x), y(1) {}
    fraction(long long a, long long b) {
        long long divisor = __gcd(a, b);
        x = a / divisor;
        y = b / divisor;
    }

    long double get() const {
        return (long double)x / (long double)y;
    }


    fraction operator+(const fraction &other) const {
        long long divisor = __gcd(x * other.y + y * other.x, y * other.y);
        return {(x * other.y + y * other.x) / divisor, y * other.y / divisor};
    }
    fraction operator-(const fraction &other) const {
        long long divisor = __gcd(x * other.y - y * other.x, y * other.y);
        return {(x * other.y - y * other.x) / divisor, y * other.y / divisor};
    }
    fraction operator*(const fraction &other) const {
        long long ret_x = x * other.x;
        long long ret_y = y * other.y;
        long long divisor = __gcd(ret_x, ret_y);
        return {ret_x / divisor, ret_y / divisor};
    }
    fraction operator/(const fraction &other) const {
        long long ret_x = x * other.y;
        long long ret_y = y * other.x;
        long long divisor = __gcd(ret_x, ret_y);
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
    fraction operator+(const long long &other) const {return (*this) + fraction(other);}
    fraction operator-(const long long &other) const {return (*this) - fraction(other);}
    fraction operator*(const long long &other) const {return (*this) * fraction(other);}
    fraction operator/(const long long &other) const {return (*this) / fraction(other);}
    fraction operator+=(const long long &other) {(*this) += fraction(other); return *this;}
    fraction operator-=(const long long &other) {(*this) -= fraction(other); return *this;}
    fraction operator*=(const long long &other) {(*this) *= fraction(other); return *this;}
    fraction operator/=(const long long &other) {(*this) /= fraction(other); return *this;}
    bool operator==(const long long &other) const {return (*this) == fraction(other);}
    bool operator!=(const long long &other) const {return (*this) != fraction(other);}
    bool operator<(const long long &other) const {return (*this) < fraction(other);}
    bool operator>(const long long &other) const {return (*this) > fraction(other);}
    bool operator<=(const long long &other) const {return (*this) <= fraction(other);}
    bool operator>=(const long long &other) const {return (*this) >= fraction(other);}

    fraction operator-() const {return fraction(0)-(*this);}

    void print() {
        cout << x << "/" << y << endl;
    }
};
//負の数に注意!!