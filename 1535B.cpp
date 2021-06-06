#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vi a(n);
		for(int &x : a) cin >> x;
		sort(a.begin(), a.end(), [](int i, int j) { return (i&1) < (j&1); });
		int ans = 0;
		for(int j = 0; j < n; ++j) {
			for(int i = 0; i < j; ++i)
				if(__gcd(a[i], 2*a[j]) > 1)
					++ ans;
		}
		cout << ans << '\n';
	}

	return 0;
}