#include <iostream>

#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/arith_utils.h"

using namespace std;
typedef mint<int(1e9+7)> mi;
AU<mi> utils;

int main() {
	int n;
	cin >> n;
	mi ans = 0;
	for(int k = n&1; 2*k <= n; k += 2) {
		ans += utils.C(n-k, k) * utils.fact(n-k);
		if(k > 0) ans += utils.C(n-k-1, k-1) * utils.fact(n-k);
	}
	cout << 2*ans << endl;
	return 0;
}