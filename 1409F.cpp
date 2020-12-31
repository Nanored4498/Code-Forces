#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	string s, t;
	cin >> s >> t;

	if(t[0] == t[1]) {
		int C = k;
		for(char c : s) if(c == t[0]) ++ C;
		C = min(C, n);
		cout << (C*(C-1)) / 2 << "\n";
	} else {
		vvi dp(1, vi(1, 0));
		for(int i = 0; i < n; ++i) {
			vvi dp2(i+2, vi(std::min(k+1, i+2), -1));
			for(int a = 0; a < dp2.size(); ++a) {
				for(int m = 0; m < dp2[a].size(); ++m) {
					if(m < dp[0].size()) { // don't apply move
						if(s[i] == t[0]) {
							if(a > 0) dp2[a][m] = std::max(dp2[a][m], dp[a-1][m]);
						} else if(s[i] == t[1]) {
							if(a < dp.size() && dp[a][m] != -1) dp2[a][m] = std::max(dp2[a][m], dp[a][m] + a);
						} else if(a < dp.size())
							dp2[a][m] = std::max(dp2[a][m], dp[a][m]);
					}
					if(m > 0) { // apply move
						if(a > 0) dp2[a][m] = std::max(dp2[a][m], dp[a-1][m-1]);
						if(a < dp.size() && dp[a][m-1] != -1) dp2[a][m] = std::max(dp2[a][m], dp[a][m-1] + a);
					}
				}
			}
			swap(dp, dp2);
		}
		int ans = 0;
		for(const vi &v : dp)
			for(int x : v)
				ans = max(ans, x);
		cout << ans << "\n";
	}

	return 0;
}