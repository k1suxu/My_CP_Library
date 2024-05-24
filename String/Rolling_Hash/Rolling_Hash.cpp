struct Rolling_Hash {
    static const unsigned long long rh_mod = (1ull << 61ull) - 1;
    vector<unsigned long long> power;
    unsigned long long base;

    static inline unsigned long long add(unsigned long long a, unsigned long long b) {
        if((a += b) >= rh_mod) a -= rh_mod;
        return a;
    }

    static inline unsigned long long mul(unsigned long long a, unsigned long long b) {
        __uint128_t c = (__uint128_t) a * b;
        return add(c >> 61, c & rh_mod);
    }

    static inline unsigned long long generate_base() {
        mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution< unsigned long long > rand(1, Rolling_Hash::rh_mod - 1);
        return rand(mt);
    }

    inline void expand(size_t sz) {
        if((int)power.size() < (int)sz + 1) {
            int pre_sz = (int) power.size();
            power.resize(sz + 1);
            for(int i = pre_sz - 1; i < (int)sz; i++) {
                power[i + 1] = mul(power[i], base);
            }
        }
    }

    Rolling_Hash() {
        base = generate_base();
        power = {1};
    }

    // vector<unsigned long long> make_hash(const string &s) const {
    //     int sz = s.size();
    //     vector<unsigned long long> hashed(sz + 1);
    //     for(int i = 0; i < (int)sz; i++) {
    //         hashed[i + 1] = add(mul(hashed[i], base), s[i]);
    //     }
    //     return hashed;
    // }
    // template<typename T>
    // vector<unsigned long long> make_hash(const vector<T> &s) const {
    //     int sz = s.size();
    //     vector<unsigned long long> hashed(sz + 1);
    //     for(int i = 0; i < (int)sz; i++) {
    //         hashed[i + 1] = add(mul(hashed[i], base), s[i]);
    //     }
    //     return hashed;
    // }
    // vector<unsigned long long> make_hash_string(const string &s) const {
    //     vector<char> ss;
    //     for(char c : s) ss.push_back(c);
    //     return make_hash<char>(ss);
    // }
    template<typename T_String>
    vector<unsigned long long> make_hash(const T_String &s) const {
        int sz = (int)s.size();
        vector<unsigned long long> hashed(sz + 1);
        for(int i = 0; i < (int)sz; i++) {
            hashed[i + 1] = add(mul(hashed[i], base), s[i]);
        }
        return hashed;
    }

    unsigned long long query(const vector<unsigned long long> &s, int l, int r) {
        expand(r - l);
        return add(s[r], rh_mod - mul(s[l], power[r - l]));
    }

    unsigned long long combine(unsigned long long h1, unsigned long long h2, size_t h2len) {
        expand(h2len);
        return add(mul(h1, power[h2len]), h2);
    }

    int lcp(const vector<unsigned long long> &a, int l1, int r1, const vector<unsigned long long> &b, int l2, int r2) {
        int len = min(r1 - l1, r2 - l2);
        int low = 0, high = len + 1;
        while(high - low > 1) {
            int mid = (low + high) / 2;
            if(query(a, l1, l1 + mid) == query(b, l2, l2 + mid)) low = mid;
            else high = mid;
        }
        return low;
    }

    bool issame(const vector<unsigned long long> &a, int l1, int r1, const vector<unsigned long long> &b, int l2, int r2) {
        assert(r1-l1 == r2-l2);
        return query(a, l1, r1) == query(b, l2, r2);
    }
    bool issame(const vector<unsigned long long> &a, int l1, const vector<unsigned long long> &b, int l2, int len) {
        return issame(a, l1, l1+len, b, l2, l2+len);
    }

    //0: 1の方がでかい, 1: 2の方がでかい, 2: 同じ
    int string_compare(const string &raw1, const vector<unsigned long long> &hash1, int l1, int r1, const string &raw2, const vector<unsigned long long> &hash2, int l2, int r2) {
        int LCP = lcp(hash1, l1, r1, hash2, l2, r2);
        if(LCP == min(r1 - l1, r2 - l2)) {
            if(r1-l1 == r2-l2) return 2;
            else if(r1-l1 == LCP) return 1;
            else return 0;
        }
        //0: l1の方がでかい, 1: l2の方がでかい
        return raw1[l1+LCP] < raw2[l2+LCP];
    }

    //0: 1の方がでかい, 1: 2の方がでかい, 2: 同じ
    template<typename T>
    int compare(const vector<T> &raw1, const vector<unsigned long long> &hash1, int l1, int r1, const vector<T> &raw2, const vector<unsigned long long> &hash2, int l2, int r2) {
        int LCP = lcp(hash1, l1, r1, hash2, l2, r2);
        if(LCP == min(r1 - l1, r2 - l2)) {
            if(r1-l1 == r2-l2) return 2;
            else if(r1-l1 == LCP) return 1;
            else return 0;
        }
        //0: l1の方がでかい, 1: l2の方がでかい
        return raw1[l1+LCP] < raw2[l2+LCP];
    }

    // is verified ??
    bool is_palindrome(const vector<unsigned long long> normal, const vector<unsigned long long> rev, int l, int r) {
        const int hash_len = (int)normal.size();
        assert((int)rev.size() == hash_len);
        // cout << l << " " << r << " " << hash_len-r << " " << hash_len-l << endl;
        // hash_len-r, hash_len-lじゃないとだめじゃない？なぜうまくいくんだ？開区間前提だっけか？？
        return issame(normal, l, r, rev, hash_len-1-r, hash_len-1-l);
    }
};