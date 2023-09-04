#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string s;
		cin >> s;
		int o = 1;
		int z = 1e9;
		int n = 0;
		bool ok = true;
		for(char c : s) {
			if(c == '+') {
				++n;
			} else if(c == '-') {
				if(--n < z) z = 1e9;
				o = max(1, min(o, n));
			} else if(c == '1') {
				if(z <= n) ok = false;
				o = max(1, n);
			} else /* 0 */ {
				if(o >= n) ok = false;
				z = min(z, n);
			}
		}
		cout << (ok ? "YES\n" : "NO\n");
	}

	return 0;
}