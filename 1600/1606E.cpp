#include <iostream>
#include <vector>

#include "../CPHeaders/mod_arith.h"
#include "../CPHeaders/arith_utils.h"

using namespace std;
const int MOD = 998244353;
typedef mint<MOD> mi;
typedef vector<mi> vi;
AU<mi> utils;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, x;
	cin >> n >> x;
	vector<vi> dp(n+1, vi(x+1, 0));
	for(int j = 1; j <= x; ++j) dp[0][j] = 1;
	for(int i = 2; i <= n; ++i)
		for(int j = 1; j <= x; ++j) {
			if(i > j) dp[i][j] = pow(mi(j), i);
			else {
				dp[i][j] = 0;
				for(int k = 0; k <= i; ++k) {
					dp[i][j] += pow(mi(i-1), k) * utils.C(i, k) * dp[i-k][j-i+1];
				}
			}
		}
	cout << dp[n][x] << '\n';

	return 0;
}