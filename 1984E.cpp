#include <iostream>
#include <vector>
#include <array>

using namespace std;
using ll = long long;
using vi = vector<int>;

vector<vi> G;

// first: not using u in MIS
// second: using u in MIS
// third: not using u in MIS and using one of his leaves as root
// fourth: using u in MIS and using one of its leaves as root
array<int, 4> f(int u, int u0=-1) {
	array<int, 4> ans = {0, 1, 0, 0};
	int add0 = 0, add1 = 0;
	for(int v : G[u]) if(v != u0) {
		if(G[v].size() == 1) {
			++ ans[0];
			add1 = max(add1, 1);
		} else {
			auto [x, y, z, t] = f(v, u);
			ans[0] += max(x, y);
			ans[1] += x;
			add0 = max(add0, max(z, t) - max(x, y));
			add1 = max(add1, z-x);
		}
	}
	ans[2] = ans[0] + add0;
	ans[3] = ans[1] + add1;
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		G.clear();
		G.resize(n);
		for(int i = 1; i < n; ++i) {
			int u, v;
			cin >> u >> v;
			--u; --v;
			G[u].push_back(v);
			G[v].push_back(u);
		}
		if(n == 2) {
			cout << "2\n";
		} else {
			int r = 0;
			while(G[r].size() == 1) ++r;
			auto a = f(r);
			int ans = 0;
			for(int x : a) ans = max(ans, x);
			cout << ans << '\n';
		}
	}

	return 0;
}