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
		int n;
		cin >> n;
		vi w(n);
		ll ans = 0;
		for(int i = 0; i < n; ++i) {
			cin >> w[i];
			ans += w[i];
		}
		vector<bool> seen(n, false);
		vi add;
		for(int i = 0; i < 2*n-2; ++i) {
			int x;
			cin >> x;
			-- x;
			if(seen[x]) add.push_back(w[x]);
			else seen[x] = true;
		}
		sort(add.rbegin(), add.rend());
		cout << ans;
		for(int x : add) {
			ans += x;
			cout << " " << ans;
		}
		cout << "\n";
	}

	return 0;
}