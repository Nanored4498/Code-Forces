#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

vector<vector<int>> G;
vector<int> md, add;

int dfs0(int i, int p=-1) {
	md[i] = 1;
	for(int j : G[i]) if(j != p) md[i] = max(md[i], 1+dfs0(j, i));
	if(md[i] > 1) for(int j : G[i]) if(j != p && md[j]+1==md[i]) {
		add[j] = 1;
		break;
	}
	return md[i];
}

void dfs(int i, int p=-1) {
	for(int j : G[i]) if(j != p) {
		if(add[j]) {
			add[j] += add[i];
			add[i] = -1;
		}
		dfs(j, i);
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll n, k;
	cin >> n >> k;
	G.resize(n);
	md.resize(n);
	add.assign(n, 0);
	for(int i = 1; i < n; ++i) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		G[a].push_back(b);
		G[b].push_back(a);
	}
	dfs0(0);
	dfs(0);
	sort(add.rbegin(), add.rend());
	ll w = 0, ans = -n*n;
	for(ll r = 1; r <= k; ++r) {
		w += add[r-1];
		ll b = min(n-r-w, n/2);
		ll wb = n-r-b;
		ans = max(ans, wb*(r-b));
	}
	cout << ans << endl;

	return 0;
}