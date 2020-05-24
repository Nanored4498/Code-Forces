#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<ll> vll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, m;
	cin >> t;
	while(t --> 0) {
		cin >> n >> m;
		vector<vll> a(n, vll(m));
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j) {
				cin >> a[i][j];
				a[i][j] -= i + j;
			}
		ll res = INT64_MAX;
		for(int i0 = 0; i0 < n; ++i0) {
			for(int j0 = 0; j0 < m; ++j0) {
				ll x = a[i0][j0];
				if(x > a[0][0]) continue;
				vll d(m+1, INT64_MAX);
				d[1] = 0;
				for(int i = 0; i < n; ++i) {
					for(int j = 0; j < m ; ++j) {
						if(a[i][j] < x) d[j+1] = INT64_MAX;
						else {
							d[j+1] = min(d[j], d[j+1]);
							ll add = min(INT64_MAX - d[j+1], a[i][j] - x);
							d[j+1] += add;
						}
					}
				}
				res = min(res, d[m]);
			}
		}
		cout << res << "\n";
	}

	return 0;
}