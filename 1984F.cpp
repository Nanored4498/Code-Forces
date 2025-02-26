#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;
using ll = long long;

const ll MOD = 998244353;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		ll m;
		cin >> n >> m;
		string s(n+2, 'P');
		s.back() = 'S';
		vector<ll> b(n+2, 0);
		for(int i = 1; i <= n; ++i) cin >> s[i];
		for(int i = 1; i <= n; ++i) cin >> b[i];

		ll ans = 0;
		unordered_set<ll> SS;
		for(int i0 = 1; i0 <= n+1; ++i0) {
			const ll sum = b[i0-1] + b[i0];
			if(SS.count(sum)) continue;
			SS.insert(sum);
			pair<ll, ll> dp = {1, 0};
			for(int i = 1; i <= n+1; ++i) {
				pair<ll, ll> dp2 = {0, 0};
				if(s[i] != 'S') {
					if(abs(b[i]-b[i-1]) <= m) // "PP"
						dp2.first = (dp2.first + dp.first) % MOD;
					if(abs(b[i]+b[i-1] - sum) <= 2*m) // "SP" 
						dp2.first = (dp2.first + dp.second) % MOD;
				}
				if(s[i] != 'P') {
					if(b[i-1]+b[i] == sum) // "PS" 
						dp2.second = (dp2.second + dp.first) % MOD;
					if(abs(b[i]-b[i-1]) <= m) // "SS"
						dp2.second = (dp2.second + dp.second) % MOD;
				}
				dp = dp2;
			}
			ans = (ans + dp.second) % MOD;
		}
		cout << ans << '\n';
	}

	return 0;
}