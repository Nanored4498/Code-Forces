#include <iostream>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		string a, b;
		cin >> a >> b;
		while(a[n-1] == '.' && b[n-1]== '.') -- n;
		int i = 0;
		while(a[i] == '.' && b[i] == '.') ++i;
		int dp[3], dp2[3];
		if(a[i] == '*' && b[i] == '.') dp[0] = 0, dp[1] = 1, dp[2] = 1e9;
		if(a[i] == '.' && b[i] == '*') dp[0] = 1, dp[1] = 0, dp[2] = 1e9;
		if(a[i] == '*' && b[i] == '*') dp[0] = 1, dp[1] = 1, dp[2] = 0;
		while(++i < n) {
			for(int j = 0; j < 3; ++j) dp2[j] = 1e9;
			if(a[i] == '.' && b[i] == '.') {
				dp2[0] = min({dp[0]+1, dp[1]+2, dp[2]+2});
				dp2[1] = min({dp[0]+2, dp[1]+1, dp[2]+2});
				dp2[2] = dp[2]+2;
			} else if(a[i] == '*' && b[i] == '.') {
				dp2[0] = min({dp[0]+1, dp[1]+2, dp[2]+2});
				dp2[1] = min({dp[0]+2, dp[1]+2, dp[2]+3});
				dp2[2] = min({dp[0]+2, dp[1]+1, dp[2]+2});
			} else if(a[i] == '.' && b[i] == '*') {
				dp2[0] = min({dp[0]+2, dp[1]+2, dp[2]+3});
				dp2[1] = min({dp[0]+2, dp[1]+1, dp[2]+2});
				dp2[2] = min({dp[0]+1, dp[1]+2, dp[2]+2});
			} else if(a[i] == '*' && b[i] == '*') {
				dp2[0] = min({dp[0]+2, dp[1]+2, dp[2]+3});
				dp2[1] = min({dp[0]+2, dp[1]+2, dp[2]+3});
				dp2[2] = min({dp[0]+1, dp[1]+1, dp[2]+2});
			}
			swap(dp, dp2);
		}
		cout << min(dp[0], dp[1]) << '\n';
	}

	return 0;
}