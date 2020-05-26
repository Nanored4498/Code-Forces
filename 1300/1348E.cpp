//https://codeforces.com/problemset/problem/1348/E
#include <iostream>

using namespace std;

typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	ll a, b, r[500], r2[500];
	ll tot = 0;
	for(int l = 1; l < k; ++l) r[l] = -1;
	r[0] = 0;
	for(int i = 0; i < n; ++i) {
		cin >> a >> b;
		tot += a + b;
		for(ll l = 0; l < k; ++l) {
			r2[l] = -1;
			for(ll p = max(0LL, l-a); p < k; ++p) if(r[p] >= 0) {
				ll ra = (a+p-l)	% k;
				if(ra > a) continue;
				ll rb = ra == 0 ? 0 : k - ra;
				if(rb > b) continue;
				r2[l] = (b+r[p]-rb) % k;
				break;
			}
		}
		swap(r, r2);
	}
	ll res = 0;
	for(int l = 0; l < k; ++l) if(r[l] >= 0) res = max(res, (tot-l-r[l])/k);
	cout << res << "\n";

	return 0;
}