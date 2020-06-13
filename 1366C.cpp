#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m, a;
		cin >> n >> m;
		vector<vi> vals(n+m-1);
		for(int i = 0; i < n; ++i) {
			for(int j = 0; j < m; ++j) {
				int s = min(i+j, n+m-2-i-j);
				cin >> a;
				vals[s].push_back(a);
			}
		}
		int ans = 0;
		for(int s = 0; s < n+m-2-s; ++s) {
			sort(vals[s].begin(), vals[s].end());
			int i = 0, nv = 0;
			for(int j = 0; j < vals[s].size(); ++j) {
				if(vals[s][i] != vals[s][j]) i = j;
				nv = max(nv, j-i+1);
			}
			ans += vals[s].size() - nv;
		}
		cout << ans << "\n";
	}

	return 0;
}