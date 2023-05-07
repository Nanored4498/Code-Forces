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

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		int ans = 0, cur = 0;
		while(n--) {
			int x; cin >> x;
			if(x) cur = 0;
			else ++ cur;
			ans = max(ans, cur);
		}
		cout << ans << '\n';
	}

	return 0;
}