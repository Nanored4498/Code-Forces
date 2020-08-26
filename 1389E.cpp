#include <iostream>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		ll m, d, w;
		cin >> m >> d >> w;
		ll g = __gcd(d-1, w);
		ll x = w/g;
		ll mx = min(d, m);

		ll ans = 0;
		ll s = 1 + ((mx-1)%x);
		ans += s * ((mx-1)/x);
		mx -= s;
		ll q = mx / x;
		ans += x * (q * ((mx-1)/x) - (q*(q-1))/2);
		cout << ans << "\n";
	}

	return 0;
}