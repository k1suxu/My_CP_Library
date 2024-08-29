// Key -> long longのHasherが必要
template<typename Key, class Hasher = std::hash<Key>>
struct multiset_hash_plus {
    unordered_map<Key, long long, Hasher> table;
    mt19937_64 rng;

    multiset_hash_plus() {
        random_device rnd;
        srand((int)time(0));
        rng = mt19937_64(rnd() * rand());
    }

    vector<long long> pref_hash(vector<Key> x) {
        for(Key e : x) if(table.find(e) == table.end()) {
            table[e] = rng();
        }

        vector<long long> hashed((int)x.size() + 1, 0);
        for(int i = 0; i < (int)x.size(); i++) hashed[i+1] = hashed[i] + table[x[i]];
        return hashed;
    }

    long long get_hash(vector<long long> &hashed, int l, int r) {
        return hashed[r] - hashed[l];
    }

    long long issame(vector<long long> &hashed1, int l1, int r1, vector<long long> &hashed2, int l2, int r2) {
        return get_hash(hashed1, l1, r1) == get_hash(hashed2, l2, r2);
    }
};

// // string -> long longのHasherの例
// struct CustomHash {
//     std::size_t operator()(const std::string& key) const {
//         std::size_t hash = 0;
//         for (char c : key) {
//             hash = hash * 31 + c;
//         }
//         return hash;
//     }
// };

//ref: https://x.com/chokudai/status/875112616523218944