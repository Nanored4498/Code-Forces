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
		int n, x, y;
		cin >> n >> x >> y;
		vector<int> c(x), even, odd;
		for(int &i : c) cin >> i;
		sort(c.begin(), c.end());
		int ans = x-2;
		for(int i = 1; i < x; ++i) {
			int d = c[i] - c[i-1];
			if(d&1) odd.push_back(d);
			else even.push_back(d);
		}
		int d = n+c[0] - c.back();
		if(d&1) odd.push_back(d);
		else even.push_back(d);
		sort(even.begin(), even.end());
		sort(odd.begin(), odd.end());
		for(int d : even) {
			int sub = min(y, (d-2)/2);
			y -= sub;
			ans += 2*sub;
			d -= 2*sub;
			if(d == 2) ++ans;
		}
		for(int d : odd) {
			int sub = min(y, (d-1)/2);
			y -= sub;
			ans += 2*sub;
			d -= 2*sub;
		}
		cout << ans << '\n';
	}

	return 0;
}