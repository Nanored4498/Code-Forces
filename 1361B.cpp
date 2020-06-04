#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<ll> vl;
const ll MOD = 1e9+7;

ll pow(ll a, ll p) {
	ll pa = a;
	ll ans = 1;
	while(p > 0) {
		if(p&1) ans = (ans*pa) % MOD;
		pa = (pa*pa) % MOD;
		p >>= 1;
	}
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n;
	cin >> t;
	while(t --> 0) {
		ll p;
		cin >> n >> p;
		vl ks(n);
		for(int i = 0; i < n; ++i) cin >> ks[i];
		if(p == 1) {
			if(n&1) cout << "1\n";
			else cout << "0\n";
		} else {
			sort(ks.rbegin(), ks.rend());
			map<ll, ll> a, b;
			ll m = -1;
			ll ans = 0;
			for(ll k : ks) {
				if(m == -1) {
					++ a[k];
					ans = (ans + pow(p, k)) % MOD;
					while(a[k] == p) {
						a[k] = 0;
						++ k;
						++ a[k];
					}
					m = k;
				} else {
					++ b[k];
					ans = (ans + MOD - pow(p, k)) % MOD;
					while(b[k] == p) {
						b[k] = 0;
						++ k;
						++ b[k];
					}
					if(k == m && a[k] == b[k]) m = -1;
				}
			}
			cout << ans << "\n";
		}
	}

	return 0;
}