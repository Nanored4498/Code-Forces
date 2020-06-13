#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string s, t;
	cin >> s >> t;
	int n = s.size(), m = t.size();
	vector<vi> dp(n+1, vi(m+1, n));
	vi next(n, -1);
	for(int i = 0; i < n; ++i) if(s[i] != '.') {
		int c = 1, j = i+1;
		while(j < n) {
			if(s[j] == '.') --c;
			else ++c;
			if(c == 0) break;
			++ j;
		}
		next[i] = j==n ? -1 : j;
	}

	dp[0][0] = 0;
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j <= m; ++j) {
			dp[i+1][j] = min(dp[i+1][j], dp[i][j]+1);
			if(j < m && s[i] == t[j]) dp[i+1][j+1] = min(dp[i+1][j+1], dp[i][j]);
			if(s[i] != '.' && next[i] != -1)
				dp[next[i]+1][j] = min(dp[next[i]+1][j], dp[i][j]);
		}
	}
	cout << dp[n][m] << "\n";

	return 0;
}