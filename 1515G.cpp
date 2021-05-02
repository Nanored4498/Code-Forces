#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int n, m, q;
struct Edge {
	int v;
	ll l;
};
vector<vector<Edge>> G;
vector<bool> seen;
vector<int> idx, older;
vector<ll> path, loop;
int id = 0;

void mark(int i, ll g) {
	seen[i] = true;
	loop[i] = g;
	for(auto [j, _] : G[i]) if(!seen[j]) mark(j, g);
}

void dfs(int i, ll l) {
	path[i] = l;
	idx[i] = older[i] = ++id;
	for(auto [j, lij] : G[i]) {
		if(idx[j] == -1) dfs(j, l + lij);
		if(seen[j]) continue;
		loop[i] = __gcd(loop[i], loop[j]);
		loop[i] = __gcd(loop[i], abs(path[i] + lij - path[j]));
		older[i] = min(older[i], older[j]);
	}
	if(older[i] == idx[i]) mark(i, loop[i]);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	G.resize(n+1);
	for(int i = 0; i < m; ++i) {
		int a;
		cin >> a;
		G[a].emplace_back();
		cin >> G[a].back().v >> G[a].back().l;
	}
	cin >> q;

	seen.resize(n+1, false);
	idx.resize(n+1, -1);
	older.resize(n+1);
	path.resize(n+1);
	loop.resize(n+1, 0);
	for(int i = 1; i <= n; ++i)
		if(!seen[i]) dfs(i, 0);
	
	for(int i = 0; i < q; ++i) {
		int v;
		ll s, t;
		cin >> v >> s >> t;
		ll g = __gcd(t, loop[v]);
		if((s % g) == 0) cout << "YES\n";
		else cout << "NO\n";
	}

	return 0;
}