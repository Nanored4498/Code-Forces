#include <iostream>

using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;

pli ne[10][170];

int main() {
	for(int k = 0; k < 10; ++k) {
		ll n = 0;
		for(int p = 0; p < 17; ++p) {
			for(int l = 0; l < 10; ++l) {
				ll n2 = n+l;
				int s = 0;
				for(ll x = n2; x <= n2+k; ++x) {
					ll y = x;
					while(y > 0) {
						s += y%10;
						y /= 10;
					}
				}
				ne[k][10*p+l] = {n2, s};
			}
			n *= 10;
			n += 90;
		}
	}

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		ll ans = -1;
		for(const pli &e : ne[k]) {
			int r = n - e.second;
			if(r < 0 || r%(k+1) != 0) continue;
			r /= (k+1);
			ll p = 10;
			while(p <= e.first) p *= 10;
			ll test = e.first;
			while(r > 0) {
				int ma = test == e.first ? 8 : 9;
				int m = min(ma, r);
				test += m*p;
				r -= m;
				p *= 10;
			}
			if(ans < 0 || test < ans) ans = test;
		}
		cout << ans << "\n";
	}
	return 0;
}