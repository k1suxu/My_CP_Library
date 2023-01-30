// mod function
int mod(int a, int mod) {
    return (a%mod+mod)%mod;
}
int modpow(int a, int n, int mod) {
    int res = 1;
    while (n > 0) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}
vector<pair<int, int>> prime_factorize(int N) {
  vector<pair<int, int>> res;
  for (int a = 2; a * a <= N; ++a) {
    if (N % a != 0) continue;
    int ex = 0;
    while(N % a == 0){
      ++ex;
      N /= a;
    }
    res.push_back({a, ex});
  }
  if (N != 1) res.push_back({N, 1});
  return res;
}
int modinv(int a, int mod) {
    int b = mod, u = 1, v = 0;
    while (b) {
        int t = a/b;
        a -= t * b, swap(a, b);
        u -= t * v, swap(u, v);
    }
    u %= mod;
    if (u < 0) u += mod;
    return u;
}
int extGcd(int a, int b, int &p, int &q) {  
    if (b == 0) { p = 1; q = 0; return a; }  
    int d = extGcd(b, a%b, q, p);  
    q -= a/b * p;  
    return d;  
}
pair<int, int> ChineseRem(const vector<int> &b, const vector<int> &m) {
  int r = 0, M = 1;
  for (int i = 0; i < (int)b.size(); ++i) {
    int p, q;
    int d = extGcd(M, m[i], p, q);
    if ((b[i] - r) % d != 0) return make_pair(0, -1);
    int tmp = (b[i] - r) / d * p % (m[i]/d);
    r += M * tmp;
    M *= m[i]/d;
  }
  return make_pair(mod(r, M), M);
}
struct Comb{
  unordered_map<int,tuple<int,vector<int>,vector<int>>> mp;
  int n_,m;
  int p_, pm_;
  vector<int> ord_, fact_;
  vector<pair<int, int>> pf;
  Comb(int n,int M) : n_(n), ord_(n), fact_(n) { 
    m=M;
    pf=prime_factorize(M); 
  }
  Comb(int p, int pm, int n) :
    n_(n), p_(p), pm_(pm), ord_(n), fact_(n) {
    init(p, pm);
  }
  void init(int n) {
    ord_.resize(n);
    fact_.resize(n);
  }
  void init(long long p, long long pm) {
    p_=p,pm_=pm;
    ord_[0]=ord_[1]=0;
    fact_[0]=fact_[1]=1;
    auto&[pms,ord,fac]=mp[p];
    pms=pm;
    ord.resize(n_);
    fac.resize(n_);
    ord[0]=ord[1]=0;
    fac[0]=fac[1]=1;
    for (int i=2;i<n_;i++) {
      long long add=0;
      long long ni=i;
      while (ni % p == 0) add++,ni/=p;
      ord_[i]=ord_[i-1]+add;
      fact_[i]=fact_[ni-1]*ni%pm;
      ord[i]=ord_[i];
      fac[i]=fact_[i];
    }
  }
  void init(long long p, long long pm, int n) {
    init(n);
    init(p, pm);
  }
  void COMinit(int n){
    for(auto p : pf){
      int ps=p.first,pfs=pow(p.first,p.second);
      init(n);
      init(ps,pfs);
    }
  }
  int com(int n, int r,int p) {
    if (n<0 || r<0 || n<r) return 0;
    auto&[pms,ord,fac]=mp[p];
    int e=ord[n]-ord[r]-ord[n-r];
    int res=fac[n]*modinv(fac[r]*fac[n-r]%pms,pms)%pms;
    res=res*modpow(p,e,pms)%pms;
    return res;
  }
  int COM(int n, int k){
    if(n<0 || k<0 || n<k) return 0;
    vector<long long> vb, vm;
    for (auto ps : pf) {
        long long p = ps.first, e = ps.second;
        long long pm = pow(p,e);
        long long b = 1;
        b *= com(n, k ,p) % pm;
        b %= pm;
        vm.push_back(pm);
        vb.push_back(b);
    }
    auto res = ChineseRem(vb,vm);
    return res.first;
  }
};