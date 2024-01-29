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


	int n, m;
	cin >> n >> m;
	vector<ll> p(n);
	vector<int> x(m);
	for(ll &x : p) cin >> x;
	for(int &y : x) cin >> y;
	sort(x.begin(), x.end());
	vector<pair<int, ll>> Q;
	for(int i = 0; i < n; ++i) {
		int y = i*100;
		auto it = lower_bound(x.begin(), x.end(), y);
		int d = 1e9;
		if(it != x.end()) d = min(d, *it-y);
		if(it != x.begin()) {
			--it;
			d = min(d, y-*it);
		}
		if(!d) continue;
		Q.emplace_back(y-d, p[i]);
		Q.emplace_back(y+d, -p[i]);
	}
	ll current = 0, ans = 0;
	sort(Q.begin(), Q.end());
	for(auto [y, q] : Q) {
		current += q;
		ans = max(ans, current);
	}
	cout << ans << '\n';

	return 0;
}