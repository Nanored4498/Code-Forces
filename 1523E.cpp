#include <iostream>

#include "../CPHeaders/mod_arith.h"

using namespace std;
const int MOD = 1e9+7;
const int MAX = 1e5+5;
typedef mint<MOD> mi;

mi fact[MAX], ifact[MAX];
mi C(int n, int k) { return fact[n] * ifact[k] * ifact[n-k]; }

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	fact[0] = ifact[0] = 1;
	for(int i = 1; i < MAX; ++i) {
		fact[i] = fact[i-1] * i;
		ifact[i] = inv(fact[i]);
	}

	int t;
	cin >> t;
	while(t--) {
		int n, k, p;
		cin >> n >> k;
		mi ans = 1;
		for(int m = 1; m <= (p = n - (m-1) * (k-1)); ++m)
			ans += C(p, m) / C(n, m);
		cout << ans << '\n';
	}

	return 0;
}