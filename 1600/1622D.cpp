#include <iostream>
#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/arith_utils.h"

using namespace std;
typedef mint<998244353> mi;
AU<mi> utils;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	string s;
	cin >> n >> k >> s;
	for(char &c : s) c -= '0';
	int i = 0, j = 0, c = 0;
	while(j < n && c+s[j] <= k) c += s[j++];
	if(c < k || k == 0) {
		cout << "1\n";
		return 0;
	}
	mi ans = utils.C(j, k);
	while(j != n) {
		int j0 = j;
		++ j;
		while(j < n && !s[j]) ++j;
		while(!s[i]) ++i;
		++ i;
		ans += utils.C(j-i, k) - utils.C(j0-i, k-1);
	}
	cout << ans << endl;

	return 0;
}