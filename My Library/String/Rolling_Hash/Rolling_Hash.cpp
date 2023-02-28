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

    vector<unsigned long long> make_hash(const string &s) const {
        int sz = s.size();
        vector<unsigned long long> hashed(sz + 1);
        for(int i = 0; i < (int)sz; i++) {
            hashed[i + 1] = add(mul(hashed[i], base), s[i]);
        }
        return hashed;
    }

    template<typename T>
    vector<unsigned long long> make_hash(const vector<T> &s) const {
        int sz = s.size();
        vector<unsigned long long> hashed(sz + 1);
        for(int i = 0; i < (int)sz; i++) {
        hashed[i + 1] = add(mul(hashed[i], base), s[i]);
        }
        return hashed;
    }
    vector<unsigned long long> make_hash_string(const string &s) const {
        vector<char> ss;
        for(char c : s) ss.push_back(c);
        return make_hash<char>(ss);
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

    template<typename T>
    int compare(const vector<T> &raw, const vector<unsigned long long> &hash, int l1, int r1, int l2, int r2) {
        // repi(i, l1, r1) cout << raw[i]; cout << " ";
        // repi(i, l2, r2) cout << raw[i]; cout << "\n";
        int LCP = lcp(hash, l1, r1, hash, l2, r2);
        if(LCP == min(r1 - l1, r2 - l2)) return 2;//same
        //0: l1‚Ì•û‚ª‚Å‚©‚¢, 1: l2‚Ì•û‚ª‚Å‚©‚¢
        return raw[l1+LCP] < raw[l2+LCP];
    }

    bool is_palindrome(const vector<unsigned long long> normal, const vector<unsigned long long> rev, int l, int r) {
        const int hash_len = (int)normal.size();
        assert((int)rev.size() == hash_len);
        // cout << l << " " << r << " " << hash_len-r << " " << hash_len-l << endl;
        return issame(normal, l, r, rev, hash_len-1-r, hash_len-1-l);
    }
};