#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vl;
const ll MOD = 1e9 + 7;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vl x(n);
		vi count(60);
		ll sum = 0;
		for(int i = 0; i < n; ++i) {
			cin >> x[i];
			sum = (sum + x[i]) % MOD;
			for(int j = 0; j < 60; ++j) count[j] += (x[i] >> j) & 1LL;
		}
		ll ans = 0;
		for(int j = 0; j < n; ++j) {
			ll sAnd = 0, sOr = sum;
			for(int k = 0; k < 60; ++k) if((x[j] >> k) & 1LL) {
				sOr = (sOr + (n-count[k]) * ((1LL << k) % MOD) ) % MOD;
				sAnd = (sAnd + count[k] * ((1LL << k) % MOD)) % MOD;
			}
			ans = (ans + sAnd * sOr) % MOD;
		}
		cout << ans << "\n";
	}

	return 0;
}