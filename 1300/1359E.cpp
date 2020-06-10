#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<ll> vl;

#define MOD 998244353

ll inv(ll a) {
	ll p = MOD-2;
	ll pa = a;
	ll res = 1;
	while(p > 0) {
		if(p%2 == 1) res = (res*pa) % MOD;
		p >>= 1; 
		pa = (pa*pa) % MOD;
	}
	return res;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll n, k;
	cin >> n >> k;
	ll res = 0;
	vl fac(n+1);
	fac[0] = 1;
	for(ll i = 1; i <= n; ++i) fac[i] = (i*fac[i-1]) % MOD;
	for(ll i = 1; i <= n; ++i) {
		ll d = n / i;
		if(d < k) break;
		ll add = (fac[d-1] * inv(fac[k-1])) % MOD;
		add = (add * inv(fac[d-k])) % MOD;
		res = (res + add) % MOD;
	}
	cout << res << "\n";

	return 0;
}