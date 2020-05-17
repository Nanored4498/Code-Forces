#include <iostream>
#include <vector>

using namespace std;

#define FOR(i, n) for(int i = 0; i < (n); ++i)
typedef vector<int> VI;
typedef vector<VI> VVI;

int seen[200000], res[200000];
string ans;
bool dfs(int i, const VVI &G) {
	if(seen[i] == 1) return true;
	if(seen[i] == 2) return false;
	seen[i] = 1;
	for(int j : G[i]) {
		if(dfs(j, G)) return true;
		res[i] = min(res[i], res[j]);
	}
	seen[i] = 2;
	if(res[i] < i) ans[i] = 'E';
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, x, y;
	cin >> n >> m;
	VVI L(n), U(n);
	ans.assign(n, 'A');
	FOR(i, m) {
		cin >> x >> y;
		L[y-1].push_back(x-1);
		U[x-1].push_back(y-1);
	}
	FOR(i, n) { seen[i] = 0; res[i] = i; }
	FOR(i, n) if(seen[i] == 0 && dfs(i, L)) { cout << -1 << endl; return 0; }
	FOR(i, n) { seen[i] = 0; res[i] = i; }
	FOR(i, n) if(seen[i] == 0 && dfs(i, U)) { cout << -1 << endl; return 0; }
	int na = 0;
	FOR(i, n) if(ans[i] == 'A') ++na;
	cout << na << "\n" << ans << "\n";

	return 0;
}