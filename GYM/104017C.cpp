#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, v;
	cin >> n >> v;
	vector<ll> t(n), a(n);
	for(ll &x : t) cin >> x;
	for(ll &x : a) cin >> x;
	vector<pair<ll,ll>> xy(n);
	for(int i = 0; i < n; ++i) {
		xy[i].first = v*t[i] + a[i];
		xy[i].second = v*t[i] - a[i];
	}
	sort(xy.begin(), xy.end());
	vector<ll> smallest(1, 0.);
	for(auto [x, y] : xy) if(x >= 0) {
		auto it = upper_bound(smallest.begin(), smallest.end(), y);
		if(it == smallest.end()) smallest.push_back(y);
		else if(it != smallest.begin()) *it = y;
	}
	cout << smallest.size()-1 << '\n';

	return 0;
}