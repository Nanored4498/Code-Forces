#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	ll x, s = 0;
	cin >> n >> m >> x;
	vector<ll> a(n+1);
	for(int i = 1; i <= n; ++i) {
		cin >> a[i];
		s += a[i];
	}
	if((n-1)*x > s) return cout << "NO\n", 0;

	vector<vector<pair<int, int>>> G(n+1);
	vector<bool> seen(n+1, false);
	for(int i = 1; i <= m; ++i) {
		int x, y;
		cin >> x >> y;
		G[x].emplace_back(y, i);
		G[y].emplace_back(x, i);
	}
	cout << "YES\n";
	
	vector<int> order, tree, p, rev_ans;
	order.push_back(1);
	seen[1] = true;
	for(int i = 0; i < n; ++i) {
		for(auto [j, e] : G[order[i]]) if(!seen[j]) {
			seen[j] = true;
			order.push_back(j);
			tree.push_back(e);
			p.push_back(order[i]);
		}
	}
	for(int i = 1; i < n; ++i) {
		int u = order[n-i], v = p[n-1-i], e = tree[n-1-i];
		if(a[u]+a[v] < x) rev_ans.push_back(e);
		else {
			cout << e << '\n';
			a[v] += a[u] - x;
		}
	}
	reverse(rev_ans.begin(), rev_ans.end());
	for(int e : rev_ans) cout << e << '\n';

	return 0;
}