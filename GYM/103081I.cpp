#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
const int MAX_N = 100'000;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, M;
	cin >> N >> M;
	vector<vi> G(N);
	for(int i = 0; i < M; ++i) {
		int u, v;
		cin >> u >> v;
		-- u;
		-- v;
		G[u].push_back(v);
		G[v].push_back(u);
	}

	vector<bool> seen(N, false);
	seen[0] = true;
	vi Q, Q2;
	Q.push_back(0);
	int d = 0;
	while(!Q.empty()) {
		++ d;
		Q2.clear();
		for(int u : Q)
			for(int v : G[u])
				if(!seen[v]) {
					Q2.push_back(v);
					seen[v] = true;
				}
		swap(Q, Q2);
	}
	-- d;

	for(bool s : seen) if(!s) {
		cout << "-1\n";
		return 0;
	}
	int ans = 0;
	while((1 << ans) < d) ++ ans;
	++ ans;
	cout << ans << '\n';

	return 0;
}