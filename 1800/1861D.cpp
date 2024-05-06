#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vi a(n);
		for(int &x : a) cin >> x;
		vi d(n, 0), u(n+1, 0);
		for(int i = 1; i < n; ++i) {
			d[i] = d[i-1];
			if(a[i] >= a[i-1]) ++d[i];
		}
		for(int i = n-2; i >= 0; --i) {
			u[i] = u[i+1];
			if(a[i+1] <= a[i]) ++u[i];
		}
		int ans = u[0];
		for(int i = 0; i < n-1; ++i) {
			ans = min(ans, 1 + d[i] + u[i+1]);
		}
		cout << ans << '\n';
	}

	return 0;
}