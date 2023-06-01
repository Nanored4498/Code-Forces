#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

const int MOD = 1e9+7;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		vi a(n), b(n);
		for(int &x : a) cin >> x;
		for(int &x : b) cin >> x;
		sort(a.begin(), a.end());
		sort(b.begin(), b.end());
		ll ans = 1;
		for(int i = 0, j = 0; i < n; ++i) {
			while(j < n && b[j] < a[i]) ++j;
			if(i >= j) ans = 0;
			else ans = ans * (j-i) % MOD;
		}
		cout << ans << '\n';
	}

	return 0;
}