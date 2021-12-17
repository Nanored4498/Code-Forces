#include <iostream>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		ll n, k;
		cin >> n >> k;
		ll ans = 0, a = 1;
		while(a < n && a < k) {
			++ ans;
			a += a;
		}
		n -= a;
		ans += (n+k-1) / k;
		cout << ans << '\n';
	}

	return 0;
}