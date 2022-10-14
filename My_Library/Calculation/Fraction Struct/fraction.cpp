//Calculation
//Fraction Struct
//fraction

// x / y
struct fraction {

    int x, y;

    // a / b (既約分数になる)
    fraction(int a = 0, int b = 1) {
        int divisor = __gcd(a, b);
        x = a / divisor;
        y = b / divisor;
    }

    long double get() {
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
        return x * other.y < y * other.x;
    }
    bool operator>(const fraction &other) const {
        return x * other.y > y * other.x;
    }
    bool operator<=(const fraction &other) const {
        return x * other.y <= y * other.x;
    }
    bool operator>=(const fraction &other) const {
        return x * other.y >= y * other.x;
    }
};

//self reduce fruction : Time complexity is less than the previous version
struct fraction {
private:
    void reduce(fraction& f) {
        int divisor = __gcd(f.x, f.y);
        f = {f.x/divisor, f.y/divisor)};
    }

    void reduce(int& x, int& y) {
        int divisor = __gcd(x, y);
        x = x / divisor;
        y = y / divisor;
    }

public:

    //約分する下限
    const int when_reduce = 1000000000;
    int x, y;

    // a / b (既約分数になる)
    fraction(int a = 0, int b = 1) {
        int divisor = __gcd(a, b);
        x = a / divisor;
        y = b / divisor;
    }

    long double get() {
        return (long double)x / (long double)y;
    }



    fraction operator+(const fraction &other) const {
        int ret_x = x*other.y + y*other.x;
        int ret_y = y*other.y;
        if(ret_x >= when_reduce || ret_y >= when_reduce) reduce(ret_x, ret_y);
        return {ret_x, ret_y};
    }
    fraction operator-(const fraction &other) const {
        int ret_x = x*other.y - y*other.x);
        int ret_y = y*other.y;
        if(ret_x >= when_reduce || ret_y >= when_reduce) reduce(ret_x, ret_y);
        return {ret_x, ret_y};
    }
    fraction operator*(const fraction &other) const {
        int ret_x = x*other.x;
        int ret_y = y*other.y;
        if(ret_x >= when_reduce || ret_y >= when_reduce) reduce(ret_x, ret_y);
        return {ret_x, ret_y};
    }
    fraction operator/(const fraction &other) const {
        int ret_x = x*other.y;
        int ret_y = y*other.x;
        if(ret_x >= when_reduce || ret_y >= when_reduce) reduce(ret_x, ret_y);
        return {ret_x, ret_y};
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
        return x * other.y < y * other.x;
    }
    bool operator>(const fraction &other) const {
        return x * other.y > y * other.x;
    }
    bool operator<=(const fraction &other) const {
        return x * other.y <= y * other.x;
    }
    bool operator>=(const fraction &other) const {
        return x * other.y >= y * other.x;
    }
};
