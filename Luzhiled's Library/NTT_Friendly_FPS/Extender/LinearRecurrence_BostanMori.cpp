// [x^N]P(x)/Q(x)
// 分母分子が高々k次の時、O(k log k log N)
template <typename mint>
mint LinearRecurrence(long long k, FPS<mint> Q, FPS<mint> P) {
    Q.shrink();
    mint ret = 0;
    if (P.size() >= Q.size()) {
        auto R = P / Q;
        P -= R * Q;
        P.shrink();
        if (k < (int)R.size()) ret += R[k];
    }
    if ((int)P.size() == 0) return ret;

    P.resize((int)Q.size() - 1);
    while (k) {
        auto Q2 = Q;
        for (int i = 1; i < (int)Q2.size(); i += 2) Q2[i] = -Q2[i];
        auto S = P * Q2;
        auto T = Q * Q2;
        if (k & 1) {
            for (int i = 1; i < (int)S.size(); i += 2) P[i >> 1] = S[i];
            for (int i = 0; i < (int)T.size(); i += 2) Q[i >> 1] = T[i];
        } else {
            for (int i = 0; i < (int)S.size(); i += 2) P[i >> 1] = S[i];
            for (int i = 0; i < (int)T.size(); i += 2) Q[i >> 1] = T[i];
        }
        k >>= 1;
    }
    return ret + P[0];
}

// ref: https://nyaannyaan.github.io/library/fps/kitamasa.hpp.html