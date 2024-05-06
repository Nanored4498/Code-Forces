#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;


int i0;
vi match, seen;
vector<vi> c;

bool dfs(int i) {
	if(seen[i] == i0) return false;
	seen[i] = i0;
	for(int j : c[i]) {
		if(match[j] == -1 || dfs(match[j])) {
			match[j] = i;
			return true;
		}
	}
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	c.resize(n);
	for(vi &v : c) {
		int k;
		cin >> k;
		v.resize(k);
		for(int &x : v) {
			cin >> x;
			--x;
		}
	}

	match.assign(m, -1);
	seen.assign(n, -1);
	int ans = 0;
	for(int i = 0; i < n; ++i) {
		if(dfs(i0 = i)) ++ ans;
	}
	cout << ans << '\n';

	return 0;
}