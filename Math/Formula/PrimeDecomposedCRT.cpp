const int MAX_N = 200005;

struct PrimeCRT {
    // min_factor[i]: iの最小の素因数 (高速素因数分解用)
    int min_factor[MAX_N];
    // p_rem[p]: 素数pに関する現在の余りの制約
    // p_pow[p]: 素数pに関する現在の法(p^k)の制約
    int p_rem[MAX_N];
    int p_pow[MAX_N];

    PrimeCRT() {
        // 1. Sieveの初期化
        iota(min_factor, min_factor + MAX_N, 0);
        for (int i = 2; i * i < MAX_N; i++) {
            if (min_factor[i] == i) {
                for (int j = i * i; j < MAX_N; j += i) {
                    if (min_factor[j] == j) min_factor[j] = i;
                }
            }
        }
        // 2. 制約の初期化 (法を1とする = 制約なし)
        fill(p_pow, p_pow + MAX_N, 1);
        fill(p_rem, p_rem + MAX_N, 0);
    }

    // 新しい制約 x ≡ r (mod m) が、現在の制約と矛盾しないか確認
    bool check(int r, int m) {
        while (m > 1) {
            int p = min_factor[m];
            int cur_pe = 1;
            while (m % p == 0) {
                m /= p;
                cur_pe *= p;
            }
            
            // 既存の制約 (p_pow[p]) と新しい制約 (cur_pe) の共通部分でチェック
            // どちらか小さい方の法で余りが一致すればOK
            int common_mod = min(p_pow[p], cur_pe);
            if (p_rem[p] % common_mod != r % common_mod) {
                return false;
            }
        }
        return true;
    }

    // 新しい制約 x ≡ r (mod m) を追加・更新する
    // ※必ず check() が true であることを確認してから呼ぶこと
    void add(int r, int m) {
        while (m > 1) {
            int p = min_factor[m];
            int cur_pe = 1;
            while (m % p == 0) {
                m /= p;
                cur_pe *= p;
            }

            // 新しい制約の方が強い(法が大きい)場合、更新する
            if (cur_pe > p_pow[p]) {
                p_pow[p] = cur_pe;
                p_rem[p] = r % cur_pe;
            }
        }
    }
};