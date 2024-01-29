#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, t;
	cin >> n >> m >> t;
	vector<vector<int>> G(n);
	queue<int> Q;
	vector<int> to(n, -1);
	while(m--) {
		int a, b;
		cin >> a >> b;
		if(b == t) {
			Q.push(a);
			to[a] = a;
		} else G[b].push_back(a);
	}

	while(!Q.empty()) {
		int a = Q.front();
		Q.pop();
		for(int b : G[a]) if(to[a] != to[b]) {
			if(to[b] == -1) {
				to[b] = to[a];
			} else {
				to[b] = -2;
			}
			Q.push(b);
		}
	}

	int ans = 0;
	for(int i = 0; i < n; ++i) if(to[i] == i) ++ans;
	cout << ans << '\n';
	for(int i = 0; i < n; ++i) if(to[i] == i) cout << i << '\n';

	return 0;
}