#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, k;
		cin >> n;
		vector<ll> a(n), b(n), s(n+1);
		for(ll &x : a) cin >> x;
		for(ll &x : b) cin >> x;
		s[0] = 0;
		for(int i = 0; i < n; ++i) s[i+1] = s[i] + b[i]; 

		vector<ll> ans(n, 0), add(n, 0);
		for(int i = 0; i < n; ++i) {
			int j = i;
			auto f = [&](int j) { return s[j] - s[i]; };
			for(int m = n/2; m; m/=2) while(j+m <= n && f(j+m) < a[i]) j += m;
			++ ans[i];
			if(j < n) {
				-- ans[j];
				add[j] += min(b[j], a[i]-f(j));
			}
		}
		for(int i = 0; i < n; ++i) {
			if(i) ans[i] += ans[i-1];
			cout << ans[i]*b[i] + add[i] << ' ';
		}
		cout << '\n';
	}

	return 0;
}