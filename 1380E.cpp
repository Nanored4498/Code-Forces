#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, k;
	ll x, y;
	cin >> n >> m >> x >> k >> y;
	vi a(n), b(m);
	for(int i = 0; i < n; ++i) cin >> a[i];
	for(int j = 0; j < m; ++j) cin >> b[j];

	int i = -1;
	ll ans = 0;
	for(int j = 0; j <= m; ++j) {
		int i0 = i;
		if(j == m) i = n;
		else {
			while(i < n && a[i] != b[j]) ++ i;
			if(i >= n) return cout << "-1\n", 0;
		}
		int ni = i-i0-1;
		ll bo = 0;
		if(j > 0) bo = max(bo, (ll) b[j-1]);
		if(j < m) bo = max(bo, (ll) b[j]);
		bool sup = false;
		for(int u = i0+1; u < i; ++u) if(a[u] > bo) {
			sup = true;
			break;
		}
		if(sup) {
			if(k > ni) return cout << "-1\n", 0;
			ans += x;
			ni -= k;
		}
		if(k*y > x) {
			ans += x * ll(ni/k);
			ni %= k;
		}
		ans += y * ll(ni);
	}
	cout << ans << "\n";

	return 0;
}