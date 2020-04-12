#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
	iostream::sync_with_stdio(false);
	cout.tie(nullptr);

	int q;
	cin >> q;
	int M = 500001;
	int MS = sqrt(M)+1;
	vector<vector<int>> s;
	for(int i = 0; i < MS; i++) s.emplace_back(i, 0);
	vector<int> a(M, 0);

	int r, x, y;
	for(int i = 0; i < q; i++) {
		cin >> r >> x >> y;
		if(r == 1) {
			a[x] += y;
			for(int j = 1; j < MS; j++) s[j][x % j] += y;
		} else {
			if(x < MS) cout << s[x][y] << "\n";
			else {
				int res = 0;
				for(int j = y; j < M; j += x) res += a[j];
				cout << res << "\n";
			}
		}
	}

	return 0;
}