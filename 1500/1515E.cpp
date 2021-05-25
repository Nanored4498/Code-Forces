#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

ll M;
vector<ll> fact(500, 1), ifact(500);

ll pow(ll x, ll p) {
	ll ans = 1;
	ll px = x;
	while(p > 0) {
		if(p&1) ans = (ans * px) % M;
		px = (px * px) % M;
		p >>= 1;
	}
	return ans;
}

ll inv(ll x) {
	return pow(x, M-2);
}

ll comb(int k, int n) {
	return (((fact[n] * ifact[k]) % M) * ifact[n-k]) % M;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n >> M;
	for(int i = 1; i <= n; ++i) fact[i] = (fact[i-1] * i) % M;
	for(int i = 0; i <= n; ++i) ifact[i] = inv(fact[i]);

	ll ans = 0;
	for(int k = 1; 2*k-1 <= n; ++k) {
		int S = n - k + 1;
		ll p2S = pow(2, S-k);
		ll sm = 0;
		ll mul = 1;
		for(int j = 0; j <= k; ++j) {
			sm = (sm + mul * comb(j, k) * pow(k-j, S) + M) % M;
			mul *= -1;
		}
		ans = (ans + p2S * sm) % M;
	}

	cout << ans << '\n';

	return 0;
}