#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<ll> h(n);
	for(ll &x : h) cin >> x;
	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	sort(order.begin(), order.end(), [&](int i, int j) { return h[i] > h[j]; });
	set<int> t;
	t.insert(order[0]);
	ll ans = 0;
	for(int u = 1; u < n; ++u) {
		int i = order[u];
		auto it = t.lower_bound(i);
		int add = n;
		if(it != t.end()) add = min(add, abs(*it - i));
		if(it != t.begin()) add = min(add, abs(*(--it) - i));
		ans += add;
		t.insert(i);
	}
	cout << ans << endl;

	return 0;
}