template<typename T_VAL, typename T_EXP>
T_VAL modpow(T_VAL n, T_EXP m, const T_VAL MOD) {
    // // basically (default), 0^0=1
    // // 0^0 = 0にしたいなら以下の行が必要
    // if(n == 0) return 0;
    T_VAL res = 1;
    while(m > 0) {
        if(m&1) res = (res*n)%MOD;
        n = (n*n)%MOD;
        m >>= 1;
    }
    return res;
}