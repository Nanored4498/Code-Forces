#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vii;
const ll MOD = 1e9+7;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, q;
	cin >> n >> m >> q;
	vector<vii> G(n+1);
	while(m--) {
		int u, v, w;
		cin >> u >> v >> w;
		G[u].emplace_back(v, w);
		G[v].emplace_back(u, w);
	}
	vector<vi> dest(n+1, vi(n+1)), weight(n+1, vi(n+1, 0));
	for(int i = 1; i <= n; ++i) dest[0][i] = i;
	for(int l = 1; l <= n; ++l) {
		for(int i = 1; i <= n; ++i) {
			for(const pii &e : G[i]) {
				int j = e.first;
				int wj = e.second + weight[l-1][j];
				// if(wj > )
			}
		}
	}

	return 0;
}