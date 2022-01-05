#include <iostream>
#include <cmath>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		ll n, k, a[10];
		cin >> n >> k;
		for(int i = 0; i < n; ++i) cin >> a[i];
		++ k;
		ll ans = 0;
		for(int i = 1; i < n; ++i) {
			ll p10 = pow(10LL, a[i-1]);
			ll m = pow(10LL, a[i] - a[i-1]) - 1;
			ll x = min(k, m);
			k -= x;
			ans += x * p10;
		}
		ans += k * (ll) pow(10LL, a[n-1]);
		cout << ans << '\n';
	}

	return 0;
}