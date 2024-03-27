//first: floor(N/[l,r]), second: [l,r](閉区間)
vector<pair<long long, pair<long long, long long>>> floor_quotient_ranges(long long N){
	vector<pair<long long, pair<long long, long long>>> ans;
	for (long long i = 1; i * i <= N; i++){
		ans.push_back(make_pair(N / i, make_pair(i, i)));
	}
	for (long long i = N / ((long long) sqrt(N) + 1); i >= 1; i--){
		ans.push_back(make_pair(i, make_pair(N / (i + 1) + 1, N / i)));
	}
	return ans;
}
//first: ceil(N/[l,r]), second: [l,r](閉区間)
vector<pair<long long, pair<long long, long long>>> ceil_quotient_ranges(long long N){
	vector<pair<long long, pair<long long, long long>>> ans;
    int last = -1;
	for (long long i = 1; i * i < N; i++){
		ans.push_back(make_pair((N+i-1) / i, make_pair(i, i)));
        last = (N+i-1)/i;
	}
	for (long long i = sqrtl(N)+1; i >= 2; i--) if(i < last) {
        int l = (N+i-1)/i, r = (N%(i-1)==0? N/(i-1)-1: N/(i-1));
        if(l <= r) ans.push_back(make_pair(i, make_pair(l, r)));
	}
    ans.push_back(make_pair(1, make_pair(N, N)));
	return ans;
}