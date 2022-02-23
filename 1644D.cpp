#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
const ll MOD = 998244353;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	vector<bool> r, c;
	while(t--) {
		int n, m, k, q;
		cin >> n >> m >> k >> q;
		vector<pair<int, int>> xy(q);
		for(auto &[x, y] : xy) cin >> x >> y;
		r.resize(n+1, false);
		c.resize(m+1, false);
		reverse(xy.begin(), xy.end());
		int ans = 1;
		int nr=0, nc=0;
		for(auto &[x, y] : xy) if(!r[x] || !c[y]) {
			if(nr==n || nc==m) continue;
			nr += !r[x];
			nc += !c[y];
			r[x] = c[y] = true;
			ans = ((ll) ans * k) % MOD;
		}
		for(auto &[x, y] : xy) r[x] = c[y] = false;
		cout << ans << '\n';
	}

	return 0;
}