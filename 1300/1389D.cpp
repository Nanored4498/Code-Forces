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
		int n;
		ll k, a, b, x, y;
		cin >> n >> k >> a >> b >> x >> y;
		ll ans = 0, i = 0, I, l, d;
		if(x < a) {
			swap(a, x);
			swap(b, y);
		} // ====> "a <= x"
		if(b < x) d = x-b, l = y-a, I = 0;
		else d = 0, l = x-a+abs(y-b), I = n * (min(b, y) - x);
		while(i < n && I < k) {
			++ i;
			ll a = min(l, k-I);
			ans += (i > 1 ? min(a, d) : d) + a;
			I += a;
		}
		if(I < k) ans += 2*(k-I);
		cout << ans << "\n";
	}

	return 0;
}