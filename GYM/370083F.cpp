#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string s;
	int k, n;
	cin >> s >> k >> n;
	vector<vi> c(26, vi(26, 0));
	while(n--) {
		char x, y;
		cin >> x >> y >> c[x-'a'][y-'a'];
	}

	vector<vi> dp(k+1, vi(26, -1e9)), dp2=dp;
	if(k) dp[1] = vi(26, 0);
	dp[0][s[0]-'a'] = 0;
	for(int i = 1; i < s.size(); ++i) {
		int x = s[i]-'a';
		for(int l = 0; l <= k; ++l) {
			dp2[l].assign(26, -1e9);
			if(l) {
				for(int y = 0; y < 26; ++y)
					for(int z = 0; z < 26; ++z)
						dp2[l][y] = max(dp2[l][y], dp[l-1][z] + c[z][y]);
			}
			for(int z = 0; z < 26; ++z)
				dp2[l][x] = max(dp2[l][x], dp[l][z] + c[z][x]);
		}
		swap(dp, dp2);
	}

	int ans = -1e9;
	for(const vi &v : dp) for(int x : v) ans = max(ans, x);
	cout << ans << endl;

	return 0;
}