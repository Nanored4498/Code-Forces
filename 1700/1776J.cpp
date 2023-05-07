#include <iostream>
#include <vector>

using namespace std;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, k;
	cin >> n >> m >> k;
	vi c(n);
	for(int &x : c) cin >> x;
	int N = 2*n;
	vector<vi> d(N, vi(N, N+k));
	while(m--) {
		int s, t;
		cin >> s >> t;
		--s; --t;
		if(c[s] == c[t]) {
			d[2*s][2*t] = d[2*t][2*s] = 1;
			d[2*s+1][2*t+1] = d[2*t+1][2*s+1] = 1;
		} else {
			d[2*s][2*t+1] = d[2*t+1][2*s] = 1;
			d[2*s+1][2*t] = d[2*t][2*s+1] = 1;
		}
	}
	for(int i = 0; i < N; ++i) {
		d[i][i] = 0;
		d[i][i^1] = k;
	}
	for(int k = 0; k < N; ++k)
		for(int i = 0; i < N; ++i)
			for(int j = 0; j < N; ++j)
				d[i][j] = min(d[i][j], d[i][k]+d[k][j]);
	int ans = 0;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			for(int e = 0; e <= k; ++e)
				ans = max(ans, min(d[2*i][2*j]+e, d[2*i][2*j+1]+k-e));
	cout << ans << '\n';

	return 0;
}