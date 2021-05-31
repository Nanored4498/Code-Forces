#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<ll> a(n), b(n);
	for(int i = 0; i < n; ++i) cin >> a[i];
	for(int i = 0; i < n; ++i) cin >> b[i];

	ll sum0 = 0;
	for(int i = 0; i < n; ++i) sum0 += a[i]*b[i];
	ll ans = sum0;

	for(int i = 0; i < n; ++i) {
		ll sum = sum0;
		int x = i-1, y = i+1;
		while(x >= 0 && y < n) {
			sum += a[x]*(b[y] - b[x]) + a[y]*(b[x] - b[y]);
			ans = max(ans, sum);
			-- x;
			++ y;
		}
	}

	for(int i = 0; i < n; ++i) {
		ll sum = sum0;
		int x = i, y = i+1;
		while(x >= 0 && y < n) {
			sum += a[x]*(b[y] - b[x]) + a[y]*(b[x] - b[y]);
			ans = max(ans, sum);
			-- x;
			++ y;
		}
	}

	cout << ans << '\n';

	return 0;
}