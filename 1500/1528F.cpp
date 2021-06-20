#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
const ll MOD = 998244353; // MOD = 7 * 17 * 2^23 + 1

ll pow(ll x, ll p) {
	ll ans = 1;
	while(p > 0) {
		if(p&1) ans = (ans * x) % MOD;
		p >>= 1;
		x = (x*x) % MOD;
	}
	return ans;
}

ll inv(ll x) {
	return pow(x, MOD-2);
}

void FFT(vector<ll> &P, ll g, bool invert=false) {
	int n = P.size();
	g = pow(g, (MOD-1) / n);
	if(invert) g = inv(g);

	// swap indices with their mirror binary
	for(int i = 1, j = 0; i < n; ++i) {
		int bit = n >> 1;
		for(; j&bit; bit >>= 1) j ^= bit;
		j ^= bit;
		if(i < j) swap(P[i], P[j]);
	}

	// Transform
	for(int len = 2; len <= n; len <<= 1) {
		int half = len >> 1;
		ll wl = g;
		for(int i = len; i < n; i <<= 1) wl = (wl * wl) % MOD;
		for(int i = 0; i < n; i += len) {
			ll w = 1;
			for(int j = 0; j < half; ++j) {
				ll even = P[i+j];
				ll odd = (P[i+half+j] * w) % MOD;
				P[i+j] = (even + odd) % MOD;
				P[i+half+j] = (even - odd + MOD) % MOD;
				w = (w * wl) % MOD;
			}
		}
	}

	// Divide by n if inverse
	if(invert) {
		ll in = inv(n);
		for(ll &x : P) x = (x * in) % MOD;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;

	ll g = 2;
	while(true) {
		bool good = true;
		for(ll p : {2, 7, 17}) if(pow(g, (MOD-1)/p) == 1) {
			good = false;
			break;
		}
		if(good) break;
		else ++g;
	}

	int size = 1;
	while(size < 2*(k+1)) size <<= 1;
	vector<ll> P(size, 0), Q(size, 0);
	ll ifact = 1;
	for(int i = 0; i <= k; ++i) {
		P[i] = i&1 ? (MOD-ifact) % MOD : ifact;
		Q[i] = (pow(i, k) * ifact) % MOD;
		ifact = (ifact * inv(i+1)) % MOD;
	}
	FFT(P, g);
	FFT(Q, g);
	for(int i = 0; i < (int) P.size(); ++i) P[i] = (P[i] * Q[i]) % MOD;
	FFT(P, g, true);

	ll term = pow(n+1, n);
	ll inv_np1 = inv(n+1);
	ll ans = 0;
	for(int i = 1; i <= k; ++i) {
		term = (term * inv_np1) % MOD;
		term = (term * (n-i+1)) % MOD;
		ans = (ans + term * P[i]) % MOD;
	}
	cout << ans << '\n';

	return 0;
}