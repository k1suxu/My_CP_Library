int gcd_arr(vector<int> &a){
  int n = a.size();
  for(int i=n-2; i>=0; i--){
    a[i] = __gcd(a[i], a[i+1]);
  }
  return a.front();
}