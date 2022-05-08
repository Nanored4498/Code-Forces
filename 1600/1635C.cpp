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
		bool ok = true;
		for(int i = 1; i < n; ++i) ok &= a[i-1] <= a[i];
		if(ok) {
			cout << "0\n";
		} else if(a.back() < 0 || a[n-2] > a[n-1]) {
			cout << "-1\n";
		} else {
			cout << n-2 << '\n';
			for(int i = n-2; i > 0; --i) cout << i << ' ' << n-1 << ' ' << n << '\n';
		}
	}

	return 0;
}