#include <iostream>
#include <vector>

using namespace std;

#define MAX_N 1000000
#define MOD 1000000007
typedef long long ll;

ll fact[2*MAX_N+1];

ll inv(ll n) {
	ll res = 1, n2 = n;
	ll p = MOD-2;
	while(p > 0) {
		if(p&1) res = (res*n2) % MOD;
		p /= 2;
		n2 = (n2*n2) % MOD;
	}
	return res;
}

ll cat(ll n) {
	ll fn2 = (fact[n]*fact[n]) % MOD;
	return (((fact[2*n] * inv(fn2)) % MOD) * inv(n+1)) % MOD;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;
	fact[0] = 1;
	for(ll i = 1; i <= 2*N; ++i) fact[i] = (i*fact[i-1]) % MOD;
	vector<ll> Q;
	ll res = 1, v;
	for(int i = 0; i < N; ++i) {
		cin >> v;
		while(!Q.empty() && Q.back() > v) {
			ll w = Q.back(), n = 0;
			while(!Q.empty() && Q.back() == w) { ++n; Q.pop_back(); }
			res = (res * cat(n)) % MOD;
		}
		Q.push_back(v);
	}
	while(!Q.empty()) {
		ll w = Q.back(), n = 0;
		while(!Q.empty() && Q.back() == w) { ++n; Q.pop_back(); }
		res = (res * cat(n)) % MOD;
	}
	cout << res << endl;

	return 0;
}