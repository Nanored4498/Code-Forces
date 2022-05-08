#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, v;
	cin >> n >> v;
	vector<ll> t(n), a(n);
	for(ll &x : t) cin >> x;
	for(ll &x : a) cin >> x;
	vector<pair<ll, ll>> p(n);
	for(int i = 0; i < n; ++i) p[i] = {v*t[i]-a[i], v*t[i]+a[i]};
	vector<int> d(1,0);
	sort(p.begin(), p.end());
	for(auto [x, y] : p) {
		if(x < 0 || y < 0) continue;
		auto it = upper_bound(d.begin(), d.end(), y);
		if(it == d.end()) d.push_back(y);
		else if(*it > y) *it = y; 
	}
	cout << d.size()-1 << endl;

	return 0;
}