#include <iostream>
#include <vector>
#include <array>

using namespace std;

vector<int> r, s;
vector<bool> opp;

int find(int i) {
	if(r[i] == i) return i;
	int f = find(r[i]);
	opp[i] = opp[i] != opp[r[i]];
	return r[i] = f;
}

void merge(int a, int b, bool p) {
	int fa = find(a), fb = find(b);
	if(fa == fb) return;
	if(s[fa] < s[fb]) swap(fa, fb);
	r[fb] = fa;
	s[fa] += s[fb];
	opp[fb] = p != opp[a] != opp[b];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int n, m;
	cin >> n >> m;
	vector<array<int, 2>> E(m);
	vector<vector<int>> G[2];
	for(auto &g : G) g.resize(n+1);
	for(int i = 0; i < m; ++i) {
		int w;
		for(int &x : E[i]) cin >> x;
		cin >> w;
		for(int v : E[i]) G[w-1][v].push_back(i);
	}

	r.resize(m);
	for(int i = 0; i < m; ++i) r[i] = i;
	s.assign(m, 1);
	opp.assign(m, false);
	int ans = 0;
	for(int i = 1; i <= n; ++i) {
		for(auto &g : G) while(g[i].size() >= 2) {
			int a = g[i].back(), b = g[i][g[i].size()-2];
			g[i].resize(g[i].size()-2);
			bool inA = E[a][1] == i, inB = E[b][1] == i;
			merge(a, b, inA == inB);
		}
		ans += !G[0][i].empty();
		if(!G[0][i].empty() && !G[1][i].empty()) {
			int a = G[0][i][0], b = G[1][i][0];
			bool inA = E[a][1] == i, inB = E[b][1] == i;
			merge(a, b, inA == inB);
		}
	}
	for(int i = 0; i < m; ++i) find(i);
	cout << ans << endl;
	for(bool x : opp) cout << (x ? '1' : '2');
	cout << endl;

	return 0;
}