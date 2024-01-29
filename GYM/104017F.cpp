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
		int n, m;
		cin >> n >> m;
		int mr = 0, mw = 0;
		while(m--) {
			int r, w;
			cin >> r >> w;
			mr = max(mr, r);
			mw = max(mw, w);
		}
		if(mr+mw <= n) {
			cout << string(mr, 'R')+string(n-mr, 'W') << '\n';
		} else {
			cout << "IMPOSSIBLE\n";
		}
	}

	return 0;
}