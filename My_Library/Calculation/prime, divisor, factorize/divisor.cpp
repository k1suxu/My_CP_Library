vector<int> divisor(int n) {
	vector<int> ret;
	for(int i = 1; i*i <= n; i++) {
		if(n % i == 0) {
			ret.push_back(i);
			if(i * i != n) ret.push_back(n/i);
		}
	}

	//ここ消すとペアずつ表示される(ex) 1,12,2,6,3,4 的な
	sort(ret.begin(),ret.end());
	
	return ret;
}