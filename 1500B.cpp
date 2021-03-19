#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

void euclid(ll a, ll b, ll &g, ll &u, ll &v) {
	u = 1;
	v = 0;
	ll u2 = 0, v2 = 1;
	while(b != 0) {
		ll q = a / b, r = a - q*b;
		a = b;
		b = r;
		r = u;
		u = u2;
		u2 = r - q*u2;
		r = v;
		v = v2;
		v2 = r - q*v2;
	}
	g = a;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll n, m, k;
	cin >> n >> m >> k;

	ll g, u, v;
	euclid(n, m, g, u, v);
	u *= n / g;
	v *= m / g;
	ll l = (n*m) / g;
	if(u < 0) u += l;
	if(v < 0) v += l;

	ll N = 2LL*max(n, m)+1;
	vector<ll> a(N, -1);
	for(ll i = 0; i < n; ++i) {
		ll x;
		cin >> x;
		a[x] = i;
	}

	vector<ll> eq;
	for(ll j = 0; j < m; ++j) {
		ll x;
		cin >> x;
		ll i = a[x];
		if(i == -1 || (j - i) % g != 0) continue;
		eq.push_back((i*v + j*u) % l);
	}
	sort(eq.begin(), eq.end());

	ll neq = l - eq.size();
	ll ans = k / neq;
	k -= ans * neq;
	ans *= l;
	if(k == 0) {
		while(!eq.empty() && eq.back() == --l) {
			-- ans;
			eq.pop_back();
		}
	}
	int i = 0;
	while(i < eq.size() && eq[i] < k) {
		++ k;
		++ i;
	}
	ans += k;
	cout << ans << '\n';

	return 0;
}
