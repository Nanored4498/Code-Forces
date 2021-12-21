#include <iostream>
#include <vector>

#include "../CPHeaders/mod_arith.h"

using namespace std;
const int MOD = 998244353;
typedef mint<MOD> mi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<mi> x(n), sa(n+1, 0);
	mi ans = 0;
	for(int i = 0; i < n; ++i) {
		mi y;
		int s;
		cin >> x[i] >> y >> s;
		sa[i+1] = x[i] - y;
		int a = 0, b = i;
		while(a < b) {
			int c = (a+b)/2;
			if(y < x[c]) b = c;
			else a = c+1;
		}
		sa[i+1] += sa[i] - sa[a];
		if(s == 1) ans += sa[i+1];
		sa[i+1] += sa[i];
	}
	ans += x[n-1]+1;
	cout << ans << endl;

	return 0;
}