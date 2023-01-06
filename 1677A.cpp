#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n; cin >> n;
		vi p(n);
		for(int &x: p) {cin >> x; --x;}
		ll ans = 0, pred = 0;
		vi d(n); for(int i = 0; i < n; ++i) d[i] = i;
		for(int i = 0; i < n; ++i) {
			for(int j = p[i]+1; j < n; ++j) --d[j];
			ll a = 0;
			for(int j = 0; j < i; ++j) {
				ans += a*d[p[j]];
				if(p[j] < p[i]) ++ a;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}