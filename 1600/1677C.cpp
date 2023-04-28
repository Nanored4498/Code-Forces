#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n; cin >> n;
		vi a(n), b(n), g(n);
		for(int &x : a) cin >> x;
		for(int &x : b) cin >> x;
		for(int i = 0; i < n; ++i) g[a[i]-1] = b[i]-1;
		vector<bool> seen(n, false);
		vi l;
		for(int i = 0; i < n; ++i) if(!seen[i]) {
			int j = i, li = 0;
			while(!seen[j]) {
				++li;
				seen[j] = true;
				j = g[j];
			}
			if(li == 1) continue;
			if(li&1) --li;
			l.push_back(li);
		}
		sort(l.begin(), l.end());
		int x = 1, y = n;
		ll ans = 0;
		for(int e : l) {
			ans += y-x;
			int x0 = x++;
			e -= 2;
			while(e) {
				ans += y-x;
				--y;
				ans += y-x;
				++x;
				e -= 2;
			}
			ans += y-x0;
			-- y;
		}
		cout << ans << '\n';
	}

	return 0;
}