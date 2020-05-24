#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, m, a, b;
	cin >> t;
	while(t --> 0) {
		cin >> n >> m >> a >> b;
		if(n*a != m*b) {
			cout << "NO\n";
		} else {
			cout << "YES\n";
			vector<vector<char>> res(n, vector<char>(m, '0'));
			for(int i = 0; i < n*a; ++i) res[i/a][i%m] = '1';
			for(int i = 0; i < n; ++i) {
				for(int j = 0; j < m; ++j) cout << res[i][j];
				cout << "\n";
			}
		}
	}

	return 0;
}