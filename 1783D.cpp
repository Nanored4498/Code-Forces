#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
using vi = vector<int>;
const int MOD = 998244353;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int n; cin >> n;
	vi a(n);
	for(int &x : a) cin >> x;
	int S = accumulate(a.begin(), a.end(), 0);
	vi dp(2*S+1, 0), dp2(2*S+1);
	dp[S+a[1]] = 1;
	for(int i = 2; i < n; ++i) {
		dp2.assign(2*S+1, 0);
		for(int s = 0; s <= 2*S; ++s) if(dp[s]) {
			dp2[s+a[i]] = (dp2[s+a[i]] + dp[s]) % MOD;
			if(s!=S) dp2[s-a[i]] = (dp2[s-a[i]] + dp[s]) % MOD;
		}
		swap(dp, dp2);
	}
	int ans = 0;
	for(int x : dp) ans = (ans + x) % MOD;
	cout << ans << '\n';

	return 0;
}