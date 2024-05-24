class smart_sieve {
    long long L, R, M;
    std::vector<int> small;
    std::vector<std::vector<long long>> large;
    std::vector<long long> aux;

public:
    smart_sieve(long long L, long long R): L(L), R(R), M(sqrtl(R)+1) {
        small.resize(M);
        std::iota(small.begin(), small.end(), 0);
        large.resize(R-L);
        aux.assign(R-L, 1);

        for (long long i = 2; i*i < R; ++i) {
            if (small[i] < i) continue;
            small[i] = i;
            for (long long j = i*i; j < M; j += i)
                if (small[j] == j) small[j] = i;

            for (long long j = (L+i-1)/i*i; j < R; j += i) {
                long long k = j;
                do {
                    if (aux[j-L] * aux[j-L] > R) break;

                    large[j-L].push_back(i);
                    aux[j-L] *= i;
                    k /= i;
                } while (k % i == 0);
            }
        }
    }

    std::vector<long long> factor(long long n) {
        // assert(L <= n && n < R);
        std::vector<long long> res = large[n-L];
        n /= aux[n-L];
        if (n >= M) {
            res.push_back(n);
            return res;
        }
        while (n > 1) {
            res.push_back(small[n]);
            n /= small[n];
        }
        return res;
    }
};


struct fast_prime_factorize {
private:
    vector<int> MinFactor;
    vector<bool> IsPrime;
public:
    vector<int> primes;
    
    fast_prime_factorize(const int MAXN) : MinFactor(MAXN), IsPrime(MAXN) {
        for (int i = 0; i < MAXN; ++i) IsPrime[i] = true, MinFactor[i] = -1;
        IsPrime[0] = false; IsPrime[1] = false; 
        MinFactor[0] = 0; MinFactor[1] = 1;
        for (int i = 2; i < MAXN; ++i) {
            if (IsPrime[i]) {
                MinFactor[i] = i;
                primes.push_back(i);
                for (int j = i*2; j < MAXN; j += i) {
                    IsPrime[j] = false;
                    if (MinFactor[j] == -1) MinFactor[j] = i;
                }
            }
        }
    }

    vector<pair<int,int> > factorize(int n) {
        vector<pair<int,int> > res;
        while (n != 1) {
            int prime = MinFactor[n];
            int exp = 0;
            while (MinFactor[n] == prime) {
                ++exp;
                n /= prime;
            }
            res.push_back(make_pair(prime, exp));
        }
        return res;
    }

    bool is_prime(int n) {
        return IsPrime[n];
    }
    vector<int> All_Min_Factor() {
        return MinFactor;
    }
    vector<int> All_Primes() {
        return primes;
    }
}sieve(5000000);