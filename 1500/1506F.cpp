#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<pair<int, int>> rc(n);
		for(int i = 0; i < n; ++i) cin >> rc[i].first;
		for(int i = 0; i < n; ++i) cin >> rc[i].second;
		sort(rc.begin(), rc.end());
		int x = 1, y = 1, ans = 0;
		for(const auto &p : rc) {
			if(p.first - x == p.second - y) {
				if((x+y) % 2 == 0) ans += p.first - x;
			} else {
				if((x+y) % 2 == 0) -- y;
				y += p.first - x;
				ans += (y - p.second + 1) / 2;
			}
			x = p.first;
			y = p.second;
		}
		cout << ans << '\n';
	}

	return 0;
}