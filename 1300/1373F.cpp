#include <iostream>

using namespace std;
typedef long long ll;
const int MN = 1e6+5;

ll a[MN], b[MN];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		for(int i = 0; i < n; ++i) cin >> a[i];
		for(int i = 0; i < n; ++i) cin >> b[i];
		ll x = 0, sa = 0, sb = 0;
		for(int i = 0; i < n; ++i) {
			sa += a[i];
			sb += b[i];
			x = max(x, sa - sb);
		}
		if(x > b[n-1]) cout << "NO\n";
		else {
			x = min(x, a[0]);
			b[n-1] -= x;
			a[0] -= x;
			x = min(b[n-1], a[n-1]);
			b[n-1] -= x;
			a[n-1] -= x;
			bool ok = true;
			for(int i = 0; i < n-1; ++i) {
				if(a[i] > b[i]) {
					ok = false;
					break;
				} else {
					b[i] -= a[i];
					a[i] = 0;
					x = min(b[i], a[i+1]);
					b[i] -= x;
					a[i+1] -= x;
				}
			}
			if(a[n-1] > 0) ok = false;
			if(ok) cout << "YES\n";
			else cout << "NO\n";
		}
	}

	return 0;
}