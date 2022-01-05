#include <iostream>
#include <vector>
#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/ntt.h"

using namespace std;
typedef mint<998244353> mi;
const mi G = 3;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<vector<mi>> d(n, {1, -1});
	d[0][1] = 0;
	for(int i = 1; i < n; ++i) {
		int a, b;
		cin >> a >> b;
		++ d[a-1][1]; ++ d[b-1][1];
	}
	for(int a = 1; a < n; a<<=1)
		for(int i = 0; i+a < n; i+=a<<1)
			NTT_mult_inplace(d[i], d[i+a], G);
	vector<mi> f(n+1, 1);
	for(int i = 1; i <= n; ++i) f[i] = f[i-1]*i;
	mi ans = 0, mul = 1;
	for(int k = 0; k < n; ++k) {
		ans += mul * f[n-k] * d[0][k];
		mul = - mul;
	}
	cout << ans << endl;

	return 0;
}