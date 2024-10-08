// Key -> long longのHasherが必要
template<typename Key, class Hasher = std::hash<Key>>
struct Zobrist_Hash {
    unordered_map<Key, long long, Hasher> table;
    mt19937_64 rng;

    Zobrist_Hash() {
        random_device rnd;
        srand((int)time(0));
        rng = mt19937_64(rnd() * rand());
    }

    vector<long long> pref_hash(vector<Key> x) {
        for(Key e : x) if(table.find(e) == table.end()) {
            table[e] = rng();
        }

        unordered_map<Key, bool, Hasher> used;

        vector<long long> hashed((int)x.size() + 1, 0);
        for(int i = 0; i < (int)x.size(); i++) {
            if(used.find(x[i]) == used.end()) {
                hashed[i+1] = hashed[i] ^ table[x[i]];
                used[x[i]] = true;
            } else {
                hashed[i+1] = hashed[i];
            }
        }
        return hashed;
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