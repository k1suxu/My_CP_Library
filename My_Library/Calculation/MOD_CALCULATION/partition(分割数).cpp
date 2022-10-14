//NのK分割数(P>=K(N), P(N, K))を求める
template<typename T = int>
vector<vector<T>> partition(int N, int K) {
    vector<vector<T>> p(N + 1, vector<T>(K + 1, 0));
    p[0][0] = 1;
    for(int n = 1; n < N + 1; n++) {
        for(int k = 1; k < K + 1; k++) {
            if(n - k >= 0) {
                p[n][k] = (p[n - 1][k - 1] + p[n - k][k]) % mod;
            }else {
                p[n][k] = p[n - 1][k - 1];
            }
        }
    }
    return p;
}