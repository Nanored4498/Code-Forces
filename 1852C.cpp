#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		ll k;
		cin >> n >> k;
		vector<ll> a(n+1);
		for(int i = 0; i < n; ++i) {
			cin >> a[i];
			if(a[i] == k) a[i] = 0;
		}
		a[n] = 0;
		for(int i = n; i; --i) a[i] -= a[i-1];

		using QEl = pair<ll, int>;
		priority_queue<QEl> Q;

		for(int i = 0; i < n; ++i) {
			while(!Q.empty()) {
				auto [l, j] = Q.top();
				if(a[j] != -l) {
					Q.pop();
					continue;
				}
				if(l + a[i] > k) {
					Q.pop();
					a[j] += k;
					a[i] -= k;
					Q.emplace(-a[j], j);
				} else break;
			}
			Q.emplace(-a[i], i);
		}
		ll ans = 0;
		for(ll x : a) ans += abs(x);
		cout << ans/2 << '\n';
	}

	return 0;
}