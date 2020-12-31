#include <iostream>

using namespace std;
typedef long long ll;
const ll MOD = 1e9+7;

int main() {
	int n;
	cin >> n;
	ll p2 = 1, fac = 1;
	for(int i = 2; i <= n; ++i) {
		p2 = (2*p2) % MOD;
		fac = (i*fac) % MOD;
	}
	ll ans = fac - p2;
	if(ans < 0) ans += MOD;
	cout << ans << "\n";
	return 0;
}