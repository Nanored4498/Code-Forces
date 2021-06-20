#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<ll> l(n+1), r(n+1);
		for(int i = 1; i <= n; ++i) cin >> l[i] >> r[i];
		vector<vector<int>> G(n+1);
		for(int i = 1; i < n; ++i) {
			int u, v;
			cin >> u >> v;
			G[u].push_back(v);
			G[v].push_back(u);
		}
		vector<ll> dp[2];
		dp[0].resize(n+1);
		dp[1].resize(n+1);
		vector<int> parent(n+1, 0), order;
		parent[1] = 1;
		order.push_back(1);
		for(int i = 0; i < n; ++i) {
			for(int j : G[order[i]]) if(parent[j] == 0) {
				parent[j] = order[i];
				order.push_back(j);
			}
		}
		reverse(order.begin(), order.end());
		for(int i : order) {
			dp[0][i] = dp[1][i] = 0;
			for(int j : G[i]) if(parent[j] == i) {
				dp[0][i] += max(abs(l[i] - l[j]) + dp[0][j], abs(l[i] - r[j]) + dp[1][j]);
				dp[1][i] += max(abs(r[i] - l[j]) + dp[0][j], abs(r[i] - r[j]) + dp[1][j]);
			}
		}
		cout << max(dp[0][1], dp[1][1]) << '\n';
	}

	return 0;
}