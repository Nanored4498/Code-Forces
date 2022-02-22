#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		vector<int> a(n);
		for(int &x : a) cin >> x;
		int ans = 0;
		for(int i = 2; i < n; ++i) if(a[i-2] < a[i-1] && a[i-1] > a[i]) {
			++ ans;
			a[i] = i+1 < n ? max(a[i-1], a[i+1]) : a[i-1];
		}
		cout << ans << '\n';
		for(int x : a) cout << x << ' ';
		cout << '\n';
	}

	return 0;
}