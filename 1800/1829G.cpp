#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

using namespace std;
using ll = long long;
using vi = vector<int>;

const int MAX = 1e6+5;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	vector<ll> dp(MAX), sc;
	dp[0] = 0;
	int col = 0, row = 0;
	for(int n = 1; n < MAX; ++n) {
		if(col == row) {
			dp[n] = dp[n-row-1] + (ll)n*n;
			sc.push_back((ll)n*n);
			col = 0;
			++ row;
		} else if(col == 0) {
			dp[n] = (ll)n*n + sc[col];
			sc[col] += (ll)n*n;
			++ col;
		} else {
			dp[n] = dp[n-row-1] + (ll)n*n + sc[col];
			sc[col] += (ll)n*n;
			++ col;
		} 
	}

	int t; cin >> t;
	while(t--) {
		int n;
		cin >> n;
		cout << dp[n] << '\n';
	}

	return 0;
}