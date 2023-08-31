#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

vi a[1005];
vector<bool> b[1005];
int n, m;

int f(int i, int j) {
	if(i < 0 || j < 0 || i >= n || j >= m) return 0;
	if(b[i][j] || !a[i][j]) return 0;
	b[i][j] = true;
	return a[i][j] + f(i+1, j) + f(i-1, j) + f(i, j+1) + f(i, j-1);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		cin >> n >> m;
		for(int i = 0; i < n; ++i) {
			a[i].resize(m);
			b[i].assign(m, false);
			for(int &x : a[i]) cin >> x;
		}
		int ans = 0;
		for(int i = 0; i < n; ++i) for(int j = 0; j < m; ++j) if(!b[i][j] && a[i][j]) {
			ans = max(ans, f(i, j));
		}
		cout << ans << '\n';
	}

	return 0;
}