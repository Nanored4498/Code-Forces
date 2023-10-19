#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;
using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;

const int MOD = 998244353;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	vector<pii> a = {{7, 5}, {8, 6}, {5, 4}};
	sort(a.begin(), a.end());
	auto it = lower_bound(a.begin(), a.end(), pii{7, 0});
	--it;
	cout << a[0].first << ' ' << a[0].second << endl;
	cout << it->first << ' ' << it->second << endl;

	// int t;
	// cin >> t;
	// while(t--) {
	// 	string s;
	// 	cin >> s;
	// 	int a = 0, b = 1;
	// 	int len = 1;
	// 	for(int i = 1; i < (int) s.size(); ++i) {
	// 		if(s[i] == s[i-1]) {
	// 			++ len;
	// 			++ a;
	// 		} else {
	// 			b = (ll) b * len % MOD;
	// 			len = 1;
	// 		}
	// 	}
	// 	b = (ll) b * len % MOD;
	// 	for(int i = 1; i <= a; ++i) {
	// 		b = (ll) b * i % MOD;
	// 	}
	// 	cout << a << ' ' << b << '\n';
	// 	// printf("%d %d\n", a, b);
	// }
}