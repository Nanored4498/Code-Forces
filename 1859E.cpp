#include <iostream>
#include <vector>
#include <array>

using namespace std;
using ll = long long;
using vi = vector<int>;

ll f(ll x, ll y, int i) {
	if(i == 0) return x + y;
	if(i == 1) return x - y;
	if(i == 2) return - x + y;
	return - x - y;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		vector<ll> a(n), b(n);
		for(ll &x : a) cin >> x;
		for(ll &x : b) cin >> x;
		vector<ll> dp(k+1);
		vector<array<ll, 4>> ab(n, {(ll)-1e10, (ll)-1e10, (ll)-1e10, (ll)-1e10});
		for(int i = 0; i < n; ++i) {
			const ll x = a[i], y = b[i];
			for(int j = 0; j <= min(i, k); ++j) {
				for(int l = 0; l < 4; ++l) {
					ab[i-j][l] = max(ab[i-j][l], dp[j] - f(y, x, l));
				}
			}
			for(int j = 0; j < min(i+1, k); ++j) {
				for(int l = 0; l < 4; ++l) {
					dp[j+1] = max(dp[j+1], ab[i-j][l] + f(x, y, l));
				}
			}
		}
		cout << dp[k] << '\n';
	}

	return 0;
}