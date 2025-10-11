#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;

#define rep(i,n) for(int i = 0; i < (int)n; i++)
#define FOR(n) for(int i = 0; i < (int)n; i++)
#define repi(i,a,b) for(int i = (int)a; i < (int)b; i++)
#define all(x) x.begin(),x.end()
//#define mp make_pair
#define vi vector<int>
#define vvi vector<vi>
#define vvvi vector<vvi>
#define vvvvi vector<vvvi>
#define pii pair<int,int>
#define vpii vector<pair<int,int>>

template<typename T>
bool chmax(T &a, const T b) {if(a<b) {a=b; return true;} else {return false;}}
template<typename T>
bool chmin(T &a, const T b) {if(a>b) {a=b; return true;} else {return false;}}

using ll = long long;
using ld = long double;
using ull = unsigned long long;

const ll INF = numeric_limits<long long>::max() / 2;
const ld pi = 3.1415926535897932384626433832795028;
const ll mod = 998244353;
int dx[] = {1, 0, -1, 0, -1, -1, 1, 1};
int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};

#define int long long

void solve() {
    
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    solve();
    return 0;
}
