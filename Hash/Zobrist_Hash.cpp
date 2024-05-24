template<typename T>
struct Zobrist_Hash {
    map<T, int> table;
    map<T, bool> made_table;
    mt19937_64 rng;

    Zobrist_Hash() {
        random_device rnd;
        srand((int)time(0));
        rng = mt19937_64(rnd() * rand());
    }

    vector<int> pref_hash(vector<T> x) {
        for(T e : x) if(!made_table[e]) {
            table[e] = rng();
            made_table[e] = true;
        }
        vector<int> hashed((int)x.size());
        map<T, bool> seen;
        hashed[0] = table[x[0]];
        seen[x[0]] = true;
        for(int i = 1; i < (int)x.size(); i++) {
            if(!seen[x[i]]) {
                hashed[i] = hashed[i-1] ^ table[x[i]];
                seen[x[i]] = true;
            }else {
                hashed[i] = hashed[i-1];
            }
        }
        return hashed;
    }

    int hash_val(vector<int>& hashed, int l, int r) {
        return hashed[r-1] ^ (l == 0? 0 : hashed[l-1]);
    }

    bool hash_comp(vector<int>& h1, int l1, int r1, vector<int>& h2, int l2, int r2) {
        return hash_val(h1, l1, r1) == hash_val(h2, l2, r2);
    }
};

struct Zobrist_Hash {
    unordered_map<int, int> table;
    unordered_map<int, bool> made_table;
    mt19937_64 rng;

    Zobrist_Hash() {
        random_device rnd;
        srand((int)time(0));
        rng = mt19937_64(rnd() * rand());
    }

    vector<int> pref_hash(vector<int> x) {
        for(int e : x) if(!made_table[e]) {
            table[e] = rng();
            made_table[e] = true;
        }
        vector<int> hashed((int)x.size());
        unordered_map<int, bool> seen;
        hashed[0] = table[x[0]];
        seen[x[0]] = true;
        for(int i = 1; i < (int)x.size(); i++) {
            if(!seen[x[i]]) {
                hashed[i] = hashed[i-1] ^ table[x[i]];
                seen[x[i]] = true;
            }else {
                hashed[i] = hashed[i-1];
            }
        }
        return hashed;
    }

    int hash_val(vector<int>& hashed, int l, int r) {
        return hashed[r-1] ^ (l == 0? 0 : hashed[l-1]);
    }

    bool hash_comp(vector<int>& h1, int l1, int r1, vector<int>& h2, int l2, int r2) {
        return hash_val(h1, l1, r1) == hash_val(h2, l2, r2);
    }
};