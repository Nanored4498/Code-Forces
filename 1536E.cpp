#include <iostream>

#include "../CPHeaders/mod_arith.h"

using namespace std;
typedef mint<int(1e9+7)> mi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		int a = 0, b = 0;
		for(int i = 0; i < n; ++i) {
			string s;
			cin >> s;
			for(char c : s)
				if(c == '#') ++ a;
				else ++ b;
		}
		mi ans = pow(mi(2), a);
		if(b == 0) -- ans;
		cout << ans << '\n';
	}

	return 0;
}