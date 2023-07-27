#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, k; cin >> n >> k;
		int g = 0;
		vi c(n);
		for(int &x : c) {
			cin >> x;
			g = gcd(g, x);
		}
		if(n == 1 || g > 1) {
			cout << "infinity\n";
			continue;
		}
		int M = (k+1) * c[0] * c[1];
		vector<int> dp(M, 0);
		dp[0] = 1;
		for(int c : c)
			for(int i = c; i < M; ++i)
				dp[i] = min(k+1, dp[i]+dp[i-c]);
		int i = M-1;
		while(dp[i] > k) --i;
		cout << i << '\n';
	}

	return 0;
}