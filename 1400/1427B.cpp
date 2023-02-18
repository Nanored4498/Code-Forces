#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		string s;
		cin >> n >> k >> s;
		int ans = 0, lw=-10, nl=0;
		vector<int> r;
		for(int i = 0; i < n; ++i)
			if(s[i] == 'W') {
				ans += 1+(lw==i-1);
				if(lw < 0) nl = i;
				else if(lw < i-1) r.push_back(i-lw-1);
				lw = i;
			}
		if(lw < 0) {
			cout << max(0, 2*k-1) << '\n';
			continue;
		}
		nl += n-lw-1;
		sort(r.begin(), r.end());
		for(int x : r) {
			if(k >= x) {
				ans += 2*x+1;
				k -= x;
			} else nl += x;
		}
		ans += 2*min(k, nl);
		cout << ans << '\n';
	}

	return 0;
}