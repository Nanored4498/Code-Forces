#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
using vl = vector<ll>;
const ll MOD = 1e9;

vector<vl> f(const vector<vl> &x, const vector<vl> &y) {
	const int N2 = x.size();
	vector<vl> z(N2, vl(N2, 0));
	for(int a = 0; a < N2; ++a) {
		for(int b = 0; b < N2; ++b) {
			for(int c = 0; c < N2; ++c) {
				z[a][b] = (z[a][b] + x[a][c] * y[c][b]) % MOD;
			}
		}
	}
	return z;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	ll M;
	cin >> N >> M;
	const int N2 = 1<<N;

	vector<vl> x(N2, vl(N2, 0));
	for(int a = 0; a < N2; ++a) for(int b = 0; b < N2; ++b) {
		bool bad = false;
		for(int i = 0; i < N; ++i) if((a>>i)&1 && (b>>i)&1) bad = true;
		if(bad) continue;
		vector<int> dp(N+1, 1);
		for(int i = 2; i <= N; ++i) {
			dp[i] = dp[i-1];
			if(!((a>>(i-1))&1) && !((a>>(i-2))&1) && !((b>>(i-1))&1) && !((b>>(i-2))&1))
				dp[i] += dp[i-2];
		}
		x[a][b] = dp[N];
	}

	vector<vl> y(N2, vl(N2, 0));
	for(int a = 0; a < N2; ++a) y[a][a] = 1;

	while(M > 0) {
		if(M&1) y = f(y, x);
		M >>= 1;
		x = f(x, x);
	}

	cout << y[0][0] << '\n';

	return 0;
}