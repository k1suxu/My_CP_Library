struct fast_prime_factorizer {
    fast_prime_factorizer() = default;
    template<class T> T pow_mod(T A, T N, T M) {
        T res = 1 % M;
        A %= M;
        while (N) {
            if (N & 1) res = (res * A) % M;
            A = (A * A) % M;
            N >>= 1;
        }
        return res;
    }

    long long gcd(long long A, long long B) {
        A = abs(A), B = abs(B);
        if (B == 0) return A;
        else return gcd(B, A % B);
    }
        
    long long pollard(long long N) {
        if (N % 2 == 0) return 2;
        if (is_prime(N)) return N;

        auto f = [&](long long x) -> long long {
            return (__int128_t(x) * x + 1) % N;
        };
        long long step = 0;
        while (true) {
            ++step;
            long long x = step, y = f(x);
            while (true) {
                long long p = gcd(y - x + N, N);
                if (p == 0 || p == N) break;
                if (p != 1) return p;
                x = f(x);
                y = f(f(y));
            }
        }
    }

    bool is_prime(long long N) {
        if (N <= 1) return false;
        if (N == 2 || N == 3) return true;
        if (N % 2 == 0) return false;
        vector<long long> A = {2, 325, 9375, 28178, 450775,
                            9780504, 1795265022};
        long long s = 0, d = N - 1;
        while (d % 2 == 0) {
            ++s;
            d >>= 1;
        }
        for (auto a : A) {
            if (a % N == 0) return true;
            long long t, x = pow_mod<__int128_t>(a, d, N);
            if (x != 1) {
                for (t = 0; t < s; ++t) {
                    if (x == N - 1) break;
                    x = __int128_t(x) * x % N;
                }
                if (t == s) return false;
            }
        }
        return true;
    }

    vector<long long> factorize(long long N) {
        if (N == 1) return {};
        long long p = pollard(N);
        if (p == N) return {p};
        vector<long long> left = factorize(p);
        vector<long long> right = factorize(N / p);
        left.insert(left.end(), right.begin(), right.end());
        //してもしなくてもどっちでもOK
        // sort(left.begin(), left.end());
        return left;
    }
}(factorizer);

template<typename T_VAL, typename T_EXP>
T_VAL modpow(T_VAL n, T_EXP m, const T_VAL MOD) {
    // // basically (default), 0^0=1
    // // 0^0 = 0なら以下の行が必要
    // if(n == 0) return 0;
    T_VAL res = 1;
    while(m > 0) {
        if(m&1) res = (res*n)%MOD;
        n = (n*n)%MOD;
        m >>= 1;
    }
    return res;
}

// 原始根を求める
long long primitive_root_ll(long long p) {
    if (p == 2) return 1;
    auto fs = factorizer.factorize(p - 1);
    sort(begin(fs), end(fs));
    fs.erase(unique(begin(fs), end(fs)), end(fs));

    for (int g = 2;; g++) {
        int ok = 1;
        for (auto& f : fs) {
            if (modpow<__int128_t, long long>(g, (p - 1) / f, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    exit(1);
}

// mod pでのxの位数を求める　(素数mod限定のはず？？)
long long prime_mod_order(long long x, long long p, vector<long long>& primes) {
    assert(!primes.empty());

    long long exp = p-1;
    for(auto e : primes) if(modpow<__int128_t, long long>(x, exp/e, p) == 1) exp /= e;

    return exp;
}

vector<long long> prime_pow_generating_group(long long x, long long p) {
    vector<long long> group;
    long long power = x;
    group.push_back(power);
    while(power != 1) {
        power = (power * x) % p;
        group.push_back(power);
    }
    return group;
}
vector<long long> prime_pow_generating_group(long long order, long long p, long long primitive_root) {
    return prime_pow_generating_group(modpow<__int128_t, long long>(primitive_root, (p-1)/order, p), p);
}

//ref: https://nyaannyaan.github.io/library/math/primitive-root-ll.hpp
//ref: https://37zigen.com/primitive-root/

// undebugged
// [STATUS] : [UNDER CONSTRUCTION]