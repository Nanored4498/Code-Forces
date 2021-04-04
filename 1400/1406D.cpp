#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<ll> vl;

ll ans(ll d) {
	if(d < 0) return d/2;
	else return (d+1)/2;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vl a(n);
	for(int i = 0; i < n; ++i) cin >> a[i];
	for(int i = n-1; i > 0; --i) a[i] -= a[i-1];
	ll d = a[0];
	for(int i = 1; i < n; ++i) if(a[i] > 0) d += a[i];
	cout << ans(d) << "\n";

	int q;
	cin >> q;
	while(q--) {
		int l, r;
		ll x;
		cin >> l >> r >> x;
		--l;
		if(l==0 || a[l] > 0) d -= a[l];
		a[l] += x;
		if(l==0 || a[l] > 0) d += a[l];
		if(r < n) {
			if(a[r] > 0) d -= a[r];
			a[r] -= x;
			if(a[r] > 0) d += a[r];
		}
		cout << ans(d) << "\n";
	}

	return 0;
}