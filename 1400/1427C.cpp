#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef pair<int, int> pii;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int r, n;
	cin >> r >> n;
	vector<int> dp(n+1), mdp(n+1), t(n+1);
	vector<pii> p(n+1);
	dp[0] = mdp[0] = 0;
	t[0] = 0;
	p[0] = {1,1};
	for(int i = 1; i <= n; ++i) {
		cin >> t[i] >> p[i].first >> p[i].second;
		dp[i] = -1e9;
		int j = i-1;
		while(j >= 0 && t[j] + 2*(r-1) > t[i]) {
			int d = abs(p[j].first - p[i].first) + abs(p[j].second - p[i].second);
			if(t[j] + d <= t[i]) dp[i] = max(dp[i], dp[j]);
			-- j;
		}
		if(j >= 0) dp[i] = max(dp[i], mdp[j]);
		++dp[i];
		mdp[i] = max(mdp[i-1], dp[i]);
	}
	cout << mdp[n] << endl;

	return 0;
}