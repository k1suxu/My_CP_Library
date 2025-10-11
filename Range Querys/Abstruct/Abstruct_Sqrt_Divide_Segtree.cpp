// 更新O(1), 区間取得O(√N)のセグ木
// invを要求しない代わりに更新O(√N)にすることもできる(該当するバケットを再計算するイメージ)
// invを用いずにx_invを直接指定してもよい(前計算高速化など)
template <class S, S (*op)(S, S), S (*e)(), S (*inv)(S)>
struct sqrt_segtree {
    int n, bucket_size;
    vector<S> bucket, raw;
    sqrt_segtree(int n) : n(n), bucket_size((int)sqrt(n)), bucket(n/bucket_size, e()), raw(n, e()) {}
    sqrt_segtree(vector<S> init) : n((int)init.size()), bucket_size((int)sqrt(n)), bucket(n/bucket_size, e()), raw(init) {
        for (int i = 0; i < (int)bucket.size(); i++) {
            for (int j = 0; j < bucket_size; j++) {
                bucket[i] = op(bucket[i], raw[i * bucket_size + j]);
            }
        }
    }

    void set(int i, S x) {
        // invを要求しない代わりに更新O(√N)にする場合はここを変更する (1/2)
        set(i, x, inv(raw[i]));
    }
    void set(int i, S x, S x_inv) {
        assert(0 <= i && i < n);
        int bucket_id = i / bucket_size;
        if (bucket_id < (int)bucket.size()) {
            // invを要求しない代わりに更新O(√N)にする場合はここを変更する (2/2)
            bucket[bucket_id] = op(op(bucket[bucket_id], x_inv), x);
        }
        raw[i] = x;
    }

    S get(int i) {
        assert(0 <= i && i < n);
        return raw[i];
    }

    S prod(int l, int r) {
        assert(0 <= l && r <= n && l <= r);
        if (l == r) return e();
        S ret = e();
        while (l < r) {
            if (l % bucket_size == 0 && l+bucket_size <= r) {
                ret = op(ret, bucket[l/bucket_size]);
                l += bucket_size;
            } else {
                ret = op(ret, raw[l]);
                l++;
            }
        }
        return ret;
    }
};
