int divisor_count(int n) {
	int cnt = 0;
	for(int i = 1; i*i <= n; i++) {
		if(n % i == 0) {
			cnt++;
			if(i * i != n) cnt++;
		}
	}
	return cnt;
}