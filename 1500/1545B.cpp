#include <iostream>

#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/arith_utils.h"

using namespace std;
const int MOD = 998244353;
typedef mint<MOD> mi;
AU<mi> util;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		string s;
		cin >> n >> s;
		int p = 0, z = 0, l = -1;
		for(int i = 0; i < n; ++i)
			if(s[i] == '0') ++ z;
			else if(i > 0 && s[i-1] == '1' && l != i-1) {
				l = i;
				++ p;
			}
		cout << util.C(p+z, p) << '\n';
	}

	return 0;
}