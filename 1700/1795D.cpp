#include <iostream>
#include <vector>
#include <algorithm>

#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/arith_utils.h"

using namespace std;
const int MOD = 998244353;
using mi = mint<MOD>;
AU<mi> util;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	mi ans = util.C(n/3, n/6);
	for(; n; n -= 3) {
		int a[3];
		for(int i = 0; i < 3; ++i) cin >> a[i];
		sort(a, a+3);
		if(a[0] == a[1]) {
			if(a[1] == a[2]) ans *= 3;
			else ans *= 2;
		}
	}
	cout << ans << '\n';

	return 0;
}