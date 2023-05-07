#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, m; cin >> n >> m;
		vector<pair<int, int>> e(m);
		for(auto &[a, b] : e) cin >> a >> b;
		if(m == n*(n-1)/2) {
			cout << "3\n";
			for(auto [a, b] : e) {
				if(max(a, b) == 2) cout << "1 ";
				else if(min(a, b) == 1) cout << "2 ";
				else cout << "3 ";
			}
			cout << '\n';
		} else {
			vector<int> deg(n+1, 0);
			for(auto [a, b] : e) {
				++deg[a];
				++deg[b];
			}
			int i = 1;
			while(deg[i] == n-1) ++i;
			cout << "2\n";
			for(auto [a, b] : e) {
				if(a == i || b == i) cout << "1 ";
				else cout << "2 ";
			}
			cout << '\n';
		}
	}

	return 0;
}