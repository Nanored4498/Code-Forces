#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;
typedef long long ll;
typedef vector<ll> vl;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	ll h, A, B;
	cin >> n >> h >> A >> B;
	vl x(n), y(n);
	for(int i = 0; i < n; ++i) cin >> x[i] >> y[i];

	vi left(n), right(n);
	for(int i = 0; i < n; ++i) {
		long double rm = h - y[i];
		int j = i-1;
		for(; j >= 0; --j) {
			if(x[i] - x[j] > 2*rm) break;
			long double dx = x[i] - x[j], dy = h - y[j];
			long double r = dx+dy + sqrt(2*dx*dy);
			rm = min(rm, r);
		}
		left[i] = j;
		rm = h - y[i];
		j = i+1;
		for(; j < n; ++j) {
			if(x[j] - x[i] > 2*rm) break;
			long double dx = x[j] - x[i], dy = h - y[j];
			long double r = dx+dy + sqrt(2*dx*dy);
			rm = min(rm, r);
		}
		right[i] = j;
	}

	vl dp(n);
	dp[0] = A*(h-y[0]);
	for (int i = 1; i < n; i++) {
		dp[i] = INT64_MAX;
		for(int j = i-1; j > left[i]; --j) if(i < right[j] && dp[j] != INT64_MAX) {
			ll d = x[i]-x[j];
			dp[i] = min(dp[i], dp[j] + B*d*d);
		}
		if(dp[i] != INT64_MAX) dp[i] += A*(h-y[i]);
	}
	if(dp[n-1] == INT64_MAX) cout << "impossible\n";
	else cout << dp[n-1] << "\n";
	
	return 0;
}