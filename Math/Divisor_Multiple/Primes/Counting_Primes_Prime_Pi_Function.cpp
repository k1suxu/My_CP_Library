// O(N^{2/3})
// return π(x) = the number of prime numbers between 2 and N
// cf: https://judge.yosupo.jp/submission/160673
long long counting_primes(const long long N) {
    if (N <= 1) return 0;
	if (N == 2) return 1;
	const int v = sqrtl(N);
	int s = (v + 1) / 2;

    vector<int> small(s);
	for (int i = 1; i < s; i++) small[i] = i;
	vector<int> rough(s);
	for (int i = 0; i < s; i++) rough[i] = 2 * i + 1;
	vector<long long> large(s);
	for (int i = 0; i < s; i++) large[i] = (N / (2 * i + 1) - 1) / 2;
	vector<bool> skip(v + 1);

    int pc = 0;
	for (int p = 3; p <= v; p += 2) if (!skip[p]) {
		int q = p * p;
		if ((long long)q * q > N) break;
		skip[p] = true;
		for (int i = q; i <= v; i += 2 * p) skip[i] = true;
		int ns = 0;
		for (int k = 0; k < s; k++) {
			int i = rough[k];
			if (skip[i]) continue;
			long long d = (long long)i * p;
			large[ns] = large[k] - (d <= v ? large[small[d >> 1] - pc] : small[(N/d-1)>>1]) + pc;
			rough[ns++] = i;
		}
		s = ns;
		for (int i = ((v-1)>>1), j = ((v / p) - 1) | 1; j >= p; j -= 2) {
			int c = small[j >> 1] - pc;
			for (int e = (j * p) >> 1; i >= e; i--) small[i] -= c;
		}
		pc++;
	}

    large[0] += (long long)(s + 2 * (pc - 1)) * (s - 1) / 2;
	for (int k = 1; k < s; k++) large[0] -= large[k];
	for (int l = 1; l < s; l++) {
		long long q = rough[l];
		long long M = N / q;
		int e = small[(M/q-1)>>1] - pc;
		if (e < l + 1) break;
		long long t = 0;
		for (int k = l + 1; k <= e; k++)
			t += small[(M/rough[k]-1)>>1];
		large[0] += t - (long long)(e - l) * (pc + l - 1);
	}

	return large[0] + 1;
}