#include <iostream>

#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/ntt.h"

using namespace std;
typedef long long ll;
const int MOD = 998244353;
typedef mint<MOD> mi;
template<> mi mi::g = mi::computeG();

int main() {
	int n, k;
	cin >> n >> k;

	vector<mi> P(k+1), Q(k+1);
	mi ifact = 1;
	for(int i = 0; i <= k; ++i) {
		P[i] = i&1 ? -ifact : ifact;
		Q[i] = pow(mi(i), k) * ifact;
		ifact /= (i+1);
	}
	NTT_mult_inplace<mi>(P, Q);

	mi term = pow(mi(n+1), n);
	mi inv_np1 = inv(mi(n+1));
	mi ans = 0;
	for(int i = 1; i <= k; ++i) {
		term *= inv_np1 * (n-i+1);
		ans += term * P[i];
	}
	cout << ans << endl;

	return 0;
}