#include <iostream>
#include <vector>
#include <queue>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m, x;
		cin >> n >> m >> x;
		vi h(n), ans(n), b(m, 0);
		for(int i = 0; i < n; ++i) cin >> h[i];
		priority_queue<pair<int, int>> Q;
		for(int i = 0; i < m; ++i) Q.emplace(0, i);
		for(int i = 0; i < n; ++i) {
			auto [bj, j] = Q.top();
			Q.pop();
			while(b[j] != -bj) {
				bj = Q.top().first;
				j = Q.top().second;
				Q.pop();
			}
			b[j] += h[i];
			ans[i] = j+1;
			Q.emplace(-b[j], j);
		}
		cout << "YES\n";
		for(int x : ans) cout << x << ' ';
		cout << '\n';
	}

	return 0;
}