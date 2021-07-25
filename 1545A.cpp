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
		vi order(n);
		for(int i = 0; i < n; ++i) order[i] = i;
		sort(order.begin(), order.end(), [&](int i, int j) { return a[i] < a[j]; });
		bool ok = true;
		int k = 0;
		while(k < n) {
			int l = k+1;
			int x = a[order[k]];
			while(l < n && a[order[l]] == x) ++ l;
			int p[2] {0, 0};
			for(int j = k; j < l; ++j) ++ p[order[j]&1];
			if(p[k&1] != (l-k+1)/2) {
				ok = false;
				break;
			}
			k = l;
		}
		cout << (ok ? "YES" : "NO") << '\n';
	}

	return 0;
}