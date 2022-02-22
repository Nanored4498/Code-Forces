#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
const ll MOD = 998244353;
const int MAXN = 23;
const int MAXS = 3e4;

int n, s[MAXN][26], inter[MAXN][26];
vector<ll> dp;

void f(int i, int p, bool neg) {
	dp[p] = 1;
	for(int c = 0; c < 26; ++c) {
		inter[i][c] = min(inter[i][c], s[i][c]);
		dp[p] = (dp[p] * (inter[i][c]+1)) % MOD;
	}
	if(neg) dp[p] = (MOD - dp[p]) % MOD;
	for(int j = i+1; j < n; ++j) {
		for(int c = 0; c < 26; ++c) inter[j][c] = inter[i][c];
		f(j, p|(1<<j), !neg);
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n;
	int m = 1<<n;
	for(int i = 0; i < n; ++i) {
		string x;
		cin >> x;
		for(char c : x) ++ s[i][c-'a'];
	}

	dp.resize(m);
	dp[0] = 0;
	for(int i = 0; i < n; ++i) {
		for(int c = 0; c < 26; ++c) inter[i][c] = MAXS;
		f(i, 1<<i, false);
	}
	for(int i = 0; i < n; ++i)
		for(int p = 0; p < m; ++p)
			if(p&(1<<i)) dp[p] = (dp[p] + dp[p^(1<<i)]) % MOD;
	
	ll ans = 0;
	for(int p = 1; p < m; ++p) {
		ll k = 0, si = 0;
		for(int i = 0; i < n; ++i) if(p&(1<<i)) {
			++ k;
			si += i;
		}
		si += k;
		ans ^= k*si*dp[p];
	}
	cout << ans << endl;

	return 0;
}