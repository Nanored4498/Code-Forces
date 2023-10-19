#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		vector<int> a(n), h(n);
		for(int &x : a) cin >> x;
		for(int &x : h) cin >> x;
		int l = 0, r = 0, s = 0;
		int ans = 0;
		while(l < n) {
			while(r < n && s+a[r] <= k && (r==l || (h[r-1] % h[r]) == 0)) {
				s += a[r];
				++r;
			}
			ans = max(ans, r-l);
			if(r > l) s -= a[l];
			++ l;
			r = max(r, l);
		}
		cout << ans << '\n';
	}
}