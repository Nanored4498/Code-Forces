#include <iostream>
#include "../CP-Headers/ntt.h"
#include "../CP-Headers/mod_arith.h"
#include "../CP-Headers/arith_utils.h"

using namespace std;
const int MOD = 998244353;
typedef mint<MOD> mi;
const mi g = 3;
AU<mi> utils;

mi f(int n, int k) {
	k = min(n, k);
	vector<mi> P(k+1), Q(k+1);
	for(int i = 0; i <= k; ++i) {
		P[i] = i&1 ? -utils.ifact(i) : utils.ifact(i);
		Q[i] = pow(mi(i), n) * utils.ifact(i);
	}
	NTT_mult_inplace(P, Q, g);
	mi ans = 0;
	for(int i = 0; i <= k; ++i) ans += P[i];
	return ans-1;
}

int main() {
	int n, k;
	cin >> n >> k;
	if(n == 1 || k == 1) {
		cout << "1\n";
		return 0;
	}

	vector<bool> p(n+1, true);
	vector<int> m(n+1, 1);
	for(int i = 2; i <= n; ++i) if(p[i]) {
		m[i] = -1;
		for(int j = i+i; j <= n; j += i) {
			if((j/i)%i == 0) m[j] = 0;
			else m[j] = -m[j];
			p[j] = false;
		}
	}

	mi ans=0, tmp=0;
	int last=n+1;
	for(int i = 1; i <= n; ++i) if(m[i] != 0) {
		int d = (n+i-1)/i;
		ans += m[i] * (d==last ? tmp : tmp=f(d, k));
		last = d;
	}
	cout << ans << endl;

	return 0;
}