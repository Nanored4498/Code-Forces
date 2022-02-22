#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, t;
	cin >> n >> t;
	vector<int> a(n);
	for(int &x : a) cin >> x;
	int ans = 0;
	if(a[0] <= t) ans = 1;
	for(int i = 1; i < n; ++i) {
		a[i] += a[i-1];
		if(a[i] <= t) ans = i+1;
		else {
			auto it = lower_bound(a.begin(), a.begin()+i, a[i]-t);
			ans = max(ans, (int) distance(it, a.begin()+i));
		}
	}
	cout << ans << '\n';

	return 0;
}