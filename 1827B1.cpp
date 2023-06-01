#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n; cin >> n;
		vi a(n);
		for(int &x : a) cin >> x;
		ll ans = 0;
		for(int i = 0; i < n; ++i) {
			vector<array<int, 3>> dp;
			int add = 0;
			for(int j = i; j < n; ++j) {
				array<int, 3> d = {a[j], a[j], 1};
				while(!dp.empty() && dp.back()[1] > a[j]) {
					auto [mi,ma,l] = dp.back();
					dp.pop_back();
					d[0] = min(d[0], mi);
					d[1] = max(d[1], ma);
					d[2] += l;
					add -= l-1;
				}
				dp.push_back(d);
				add += d[2]-1;
				ans += add;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}