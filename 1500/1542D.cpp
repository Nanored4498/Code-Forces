#include <iostream>
#include <vector>

#include "../CPHeaders/mod_arith.h"

using namespace std;
typedef long long ll;
const int MOD = 998'244'353;
typedef mint<MOD> mi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<mi> a(n, 0);
	int N = n;
	for(mi &x : a) {
		char c;
		cin >> c;
		if(c == '+') cin >> x;
		else -- N;
	}
	if(N == 0) {
		cout << 0 << endl;
		return 0;
	}

	mi ans = 0;
	vector<vector<mi>> dp(n, vector<mi>(N, 0));
	for(int i = 0; i < n; ++i) dp[i][0] = 1;
	for(int i = 0; i < n; ++i) if(a[i] != 0) {
		for(int j = i+1; j < n; ++j) if(a[i] <= a[j]) {
			for(int k = N-1; k > 0; --k)
				dp[j][k] += dp[j][k-1];
			for(int k = 0; k < N; ++k)
				dp[i][k] += dp[i][k];
		} else if(a[j] == 0) {
			for(int k = 1; k < N; ++k)
				dp[i][k-1] += dp[i][k];
		} else {
			for(int k = 0; k < N; ++k)
				dp[j][k] += dp[j][k];
			for(int k = N-1; k > 0; --k)
				dp[i][k] += dp[i][k-1];
		}
		
		mi count = 0;
		for(mi &x : dp[i]) count += x;
		ans += count * a[i];
	} else {
		for(int j = i+1; j < n; ++j) if(a[j] != 0) {
			dp[j][0] += dp[j][0];
			for(int k = 1; k < N; ++k)
				dp[j][k-1] += dp[j][k];
		}
	}
	cout << ans << endl;

	return 0;
}