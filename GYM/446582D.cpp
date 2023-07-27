#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;
const ll MOD = 1e9+7;

ll f(const ll n, const ll k) {
	ll n2 = n;
	vector<pair<ll, int>> pe;
	for(ll p = 2; p*p <= n2; ++p) while(n2 % p == 0) {
		if(pe.empty() || pe.back().first != p) pe.emplace_back(p, 1);
		else ++ pe.back().second;
		n2 /= p;
	}
	if(n2 != 1) pe.emplace_back(n2, 1);
	vector<ll> d(1, 1LL);
	for(auto [p, e] : pe) {
		int m = d.size();
		for(int i = 0; i < m; ++i) {
			d.push_back(d[i] * p);
			for(int j = 1; j < e; ++j) d.push_back(d.back() * p);
		}
	}
	sort(d.begin(), d.end());
	int N = d.size();
	vector<ll> m(N, 0LL);
	m[0] = k;
	for(ll x : d) {
		for(int i = N-1; i >= 0; --i) {
			ll y = d[i];
			if(y > n/x) continue;;
			int j = lower_bound(d.begin(), d.end(), x*y) - d.begin();
			if(d[j] != x*y) continue;
			m[j] = (m[j] + (m[i]+1) * (x % MOD)) % MOD;
		}
	}
	return m.back();
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		ll n, k;
		cin >> n >> k;
		cout << f(n, k) << '\n';
	}

	return 0;
}