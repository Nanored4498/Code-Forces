#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<pair<int, int>> ac(n);
	for(int i = 0; i < n; ++i) cin >> ac[i].first >> ac[i].second;
	sort(ac.begin(), ac.end());
	int mac = ac[0].first + ac[0].second;
	long long ans = ac[0].second;
	for(int i = 1; i < n; ++i) {
		ans += max(0, ac[i].first - mac) + ac[i].second;
		mac = max(mac, ac[i].first + ac[i].second);
	}
	cout << ans << '\n';

	return 0;
}