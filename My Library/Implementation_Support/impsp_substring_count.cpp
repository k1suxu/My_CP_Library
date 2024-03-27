//ref: https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int MOD = 1000000007;

// res[i][c] := i 文字目以降で最初に文字 c が登場する index (存在しないときは n)
vector<vector<int> > calcNext(const string &S) {
    int n = (int)S.size();
    vector<vector<int> > res(n+1, vector<int>(26, n));
    for (int i = n-1; i >= 0; --i) {
        for (int j = 0; j < 26; ++j) res[i][j] = res[i+1][j];
        res[i][S[i]-'a'] = i;
    }
    return res;
}

// mod 1000000007 の世界で a += b する関数
void add(long long &a, long long b) {
    a += b;
    if (a >= MOD) a -= MOD;
}

int main() {
    // 入力
    string S; cin >> S;
    int n = (int)S.size();

    // 前処理配列
    vector<vector<int> > next = calcNext(S);

    // DP
    vector<long long> dp(n+1, 0);
    dp[0] = 1; // 初期化、空文字列 "" に対応
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 26; ++j) {
            if (next[i][j] >= n) continue; // 次の文字 j がもうない場合はスルー
            add(dp[next[i][j] + 1], dp[i]);
        }
    }

    // 集計
    long long res = 0;
    for (int i = 0; i <= n; ++i) add(res, dp[i]);

    cout << res << endl;
}