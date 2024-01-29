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
		int n; cin >> n;
		vector<int> m(10, -1);
		while(n--) {
			int b, d;
			cin >> b >> d;
			--d;
			m[d] = max(m[d], b);
		}
		bool bad = false;
		int ans = 0;
		for(int b : m) {
			if(b < 0) bad = true;
			else ans += b;
		}
		if(bad) cout << "MOREPROBLEMS\n";
		else cout << ans << '\n';
	}

	return 0;
}