#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef vector<ll> vl;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		int m = 2*n;
		ll ans = 0;
		vector<vl> c(m, vl(m));
		for(int i = 0; i < m; ++i) for(int j = 0; j < m; ++j) {
			cin >> c[i][j];
			if(i >= n && j >= n) ans += c[i][j];
		}
		cout << ans + min({c[0][n], c[0][m-1], c[n-1][n], c[n-1][m-1], c[n][0], c[n][n-1], c[m-1][0], c[m-1][n-1]}) << '\n';
	}

	return 0;
}