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
		int n, k;
		cin >> n >> k;
		vi a(n), w(k);
		for(int i = 0; i < n; ++i) cin >> a[i];
		for(int i = 0; i < k; ++i) cin >> w[i];
		sort(a.rbegin(), a.rend());
		sort(w.begin(), w.end());
		ll res = 0;
		int i = 0;
		while(i < k) {
			-- w[i];
			res += a[i++];
		}
		for(int j = 0; j < k; ++j) {
			if(w[j] == 0) res += a[j];
			else {
				while(w[j] > 0) {
					-- w[j];
					++ i;
				}
				res += a[i-1];
			}
		}
		cout << res << "\n";
	}

	return 0;
}