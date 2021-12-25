#include <iostream>
#include <vector>
#include <array>

using namespace std;
typedef pair<int, int> E;

vector<vector<E>> G;
vector<int> r, s;
vector<bool> opp;

int find(int i) {
	if(r[i] == i) return i;
	int f = find(r[i]);
	opp[i] = opp[i] != opp[r[i]];
	return r[i] = f;
}

bool merge(int a, int b, bool p) {
	int fa = find(a), fb = find(b);
	if(fa == fb) return p != opp[a] != opp[b];
	if(s[fa] < s[fb]) swap(fa, fb);
	r[fb] = fa;
	s[fa] += s[fb];
	opp[fb] = p != opp[a] != opp[b];
	return false;
}

void dfs0(int i, int p=0) {
	if(p) {
		int j = 0;
		while(G[i][j].first != p) ++j;
		G[i][j] = G[i].back();
		G[i].pop_back();
	}
	for(auto [j, v] : G[i]) dfs0(j, i);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		G.clear();
		G.resize(n+1);
		for(int i = 1; i < n; ++i) {
			int x, y, v;
			cin >> x >> y >> v;
			G[x].emplace_back(y, v);
			G[y].emplace_back(x, v);
		}
		dfs0(1);

		r.resize(n+1);
		s.assign(n+1, 1);
		opp.assign(n+1, false);
		for(int i = 1; i <= n; ++i) r[i] = i;
		for(int i = 1; i <= n; ++i)
			for(auto [j, v] : G[i]) if(v != -1) {
				int e = 0;
				for(; v; v>>=1) e ^= v&1;
				merge(i, j, e);
			}
		bool no = false;
		while(m--) {
			int a, b, p;
			cin >> a >> b >> p;
			no |= merge(a, b, p);
		}
		if(no) {
			cout << "NO\n";
			continue;
		}
		cout << "YES\n";
		for(int i = 1; i <= n; ++i) find(i);
		if(opp[1]) for(int i = 1; i <= n; ++i) opp[i] = !opp[i];
		for(int i = 1; i <= n; ++i)
			for(auto [j, v] : G[i])
				cout << i << ' ' << j << ' ' << (v == -1 ? opp[i]!=opp[j] : v) << '\n';
	}

	return 0;
}