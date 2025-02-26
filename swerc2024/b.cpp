#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vector<ll> d(n);
	for(ll &x : d) cin >> x;
	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	sort(order.begin(), order.end(), [&](int i, int j) { return d[i] < d[j]; });
	vector<vector<ll>> c(n, vector<ll>(n));
	for(int i = 0; i < n; ++i) {
		ll s = 0, s2 = 0;
		for(int j = i; j < n; ++j) {
			const ll m = d[order[j]];
			s += m;
			s2 += m*m;
			const int l = j-i+1;
			c[i][j] = s2 - 2*s*m + l*m*m;
		}
	}
	k = min(k, n);
	vector<vector<pair<ll, int>>> dp(k, vector<pair<ll, int>>(n));
	for(int i = 0; i < n; ++i) dp[0][i] = {c[0][i], 0};
	for(int s = 1; s < k; ++s) {
		for(int i = s; i < n; ++i) {
			dp[s][i] = {1e18, i};
			for(int j = s; j <= i; ++j) {
				const ll c2 = c[j][i] + dp[s-1][j-1].first;
				if(c2 < dp[s][i].first) {
					dp[s][i] = {c2, j};
				}
			}
		}
	}
	vector<ll> ans(n);
	int i = n-1;
	while(k--) {
		int j = dp[k][i].second;
		for(int a = j; a <= i; ++a)
			ans[order[a]] = d[order[i]];
		i = j-1;
	}
	for(ll x : ans) cout << x << ' ';
	cout << endl;

	return 0;
}