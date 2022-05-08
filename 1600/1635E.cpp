#include <iostream>
#include <vector>

using namespace std;
typedef vector<pair<int, bool>> vib;

vector<vib> G;
vector<int> LR;
vector<int> pos;

bool lr(int i, int l) {
	LR[i] = l;
	for(auto [j, t] : G[i])
		if(LR[j] == -1) {
			if(lr(j, l^1)) return true;
		} else if(LR[j] == LR[i]) return true;
	return false;
}

bool topo(int i) {
	static int p = 0;
	pos[i] = -1;
	for(auto [j, i] : G[i])
		if(pos[j] == -1) return true;
		else if(!pos[j] && topo(j)) return true;
	pos[i] = ++p;
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	G.resize(n);
	LR.assign(n, -1);
	while(m--) {
		int t, i, j;
		cin >> t >> i >> j;
		--i; --j;
		G[i].emplace_back(j, t==1);
		G[j].emplace_back(i, t==1);
	}

	for(int i = 0; i < n; ++i) if(LR[i] == -1 && lr(i, 0)) {
		cout << "NO\n";
		return 0;
	}

	pos.assign(n, 0);
	for(int i = 0; i < n; ++i) {
		vib g;
		for(auto [j, t] : G[i]) {
			if(t && LR[i]) g.emplace_back(j, t);
			if(!t && !LR[i]) g.emplace_back(j, t);
		}
		G[i] = g;
	}
	for(int i = 0; i < n; ++i) if(!pos[i] && topo(i)) {
		cout << "NO\n";
		return 0;
	}
	cout << "YES\n";
	for(int i = 0; i < n; ++i)
		cout << "LR"[LR[i]] << ' ' << pos[i] << '\n';

	return 0;
}