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
		int n, l, r;
		cin >> n >> l >> r;
		vi c(n);
		for(int i = 0; i < n; ++i) cin >> c[i];
		if(l < r) {
			reverse(c.begin(), c.end());
			swap(l, r);
		}
		vi d(n, 0);
		for(int i = 0; i < n; ++i) {
			-- c[i];
			if(i < l) ++ d[c[i]];
			else -- d[c[i]];
		}
		int ans = 0;
		for(int &x : d) {
			while(x >= 2 && l > r) {
				-- l;
				++ r;
				x -= 2;
				ans += 1;
			}
		}
		for(int x : d) if(x > 0) ans += x;
		cout << ans << '\n';
	}

	return 0;
}