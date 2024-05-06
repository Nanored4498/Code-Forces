#include <iostream>
#include <vector>
#include <map>

using namespace std;
using ll = long long;
using vi = vector<int>;

const ll MOD = 1e9+7;

ll pow(ll x, ll p) {
	ll y = 1;
	while(p) {
		if(p&1) y = y*x%MOD;
		p >>= 1;
		x = x*x%MOD;
	}
	return y;
}

ll inv(ll x) {
	return pow(x, MOD-2);
}

map<map<int,int>, ll> m[100];
ll fact[100];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	fact[0] = 1;
	for(int i = 1; i < 100; ++i) fact[i] = fact[i-1]*i % MOD; 

	int n, k, t;
	cin >> n >> k >> t;
	m[0][{{1,n}}] = 1;
	for(int u = 0; u < k; ++u) {
		for(const auto &[v, c] : m[u]) {
			auto v2 = v;
			for(auto [l, lc] : v) if(l > 1) {
				for(int l2 = 1; 2*l2 <= l; ++l2) {
					int cs = (2*l2 == l ? l2 : l) * lc;
					++ v2[l2];
					++ v2[l-l2];
					if(--v2[l] == 0) v2.erase(l);
					ll &c2 = m[u+1][v2];
					c2 = (c2 + cs * c) % MOD;
					++ v2[l];
					if(-- v2[l2]   == 0) v2.erase(l2);
					if(-- v2[l-l2] == 0) v2.erase(l-l2);
				}
			}
			for(auto [l, lc] : v) {
				if(lc > 1) {
					int cs = l * l * lc*(lc-1)/2;
					++ v2[l*2];
					if((v2[l] -= 2) == 0) v2.erase(l);
					ll &c2 = m[u+1][v2];
					c2 = (c2 + cs * c) % MOD;
					v2[l] += 2;
					if(-- v2[l*2] == 0) v2.erase(l*2);
				}
				for(auto [l2, lc2] : v) {
					if(l2 >= l) break;
					int cs = l * l2 * lc * lc2;
					++ v2[l+l2];
					if(--v2[l] == 0) v2.erase(l);
					if(--v2[l2] == 0) v2.erase(l2);
					ll &c2 = m[u+1][v2];
					c2 = (c2 + cs * c) % MOD;
					++ v2[l];
					++ v2[l2];
					if(--v2[l+l2] == 0) v2.erase(l+l2);
				}
			}
		}
	}

	vi c(n);
	while(t--) {
		for(int &x : c) {
			cin >> x;
			--x;
		}
		vector<bool> seen(n, false);
		map<int, int> v;
		for(int i = 0; i < n; ++i) if(!seen[i]) {
			int l = 0, j = i;
			while(!seen[j]) {
				seen[j] = true;
				j = c[j];
				++ l;
			}
			++v[l];
		}
		ll ans = m[k][v] * inv(fact[n]) % MOD;
		for(auto [l, lc] : v) {
			ans = ans * fact[lc] % MOD;
			ans = ans * pow(l, lc) % MOD;
		}
		cout << ans << '\n';
	}

	return 0;
}