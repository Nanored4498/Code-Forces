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
		mi n, m, x, y, x2, y2, p;
		mi ax = 1, ay = 1;
		cin >> n >> m >> x >> y >> x2 >> y2 >> p;
		mi e = 0, f = 1, l = 2*(n-1)*(m-1);
		for(mi i = 0; i < l; ++i) {
			if(x == x2 || y == y2) {
				e += i*f*p/100;
				f *= 1 - p/100;
			}
			if(x == n) ax = -1;
			else if(x == 1) ax = 1;
			if(y == m) ay = -1;
			else if(y == 1) ay = 1;
			x += ax;
			y += ay;
		}
		mi ans = e + (e + l) * f / (1 - f);
		cout << ans << '\n';
	}

	return 0;
}