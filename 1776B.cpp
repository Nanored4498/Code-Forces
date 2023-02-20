#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	ll h;
	cin >> n >> h;
	vector<ll> x(n);
	for(ll &a : x) cin >> a;
	vector<vector<ll>> dp(n, vector<ll>(n+1, h));
	for(int l = 2; l <= n; ++l) {
		for(int i = 0; i <= n-l; ++i) {
			int j = i+l;
			dp[i][j] = n*h;
			for(int k = i+1; k < j; ++k)
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
			dp[i][j] -= max(0LL, h - (x[j-1]-x[i]-1)/2);
		}
	}
	cout << dp[0][n] << endl;

	return 0;
}