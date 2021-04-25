#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<int> tag(n);
		vector<ll> s(n), dp(n, 0);
		for(int i = 0; i < n; ++i) cin >> tag[i];
		for(int i = 0; i < n; ++i) cin >> s[i];

		for(int j = 1; j < n; ++j) {
			for(int i = j-1; i >= 0; --i) {
				if(tag[i] == tag[j]) continue;
				ll add = abs(s[j] - s[i]);
				ll dpi = dp[i];
				dp[i] = max(dp[i], dp[j] + add);
				dp[j] = max(dp[j], dpi + add);
			}
		}

		ll ans = 0;
		for(ll a : dp) ans = max(ans, a);
		cout << ans << '\n';
	}

	return 0;
}