#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<ll> vl;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vl a(n);
	ll ans = 0;
	for(int i = 0; i < n; ++i) cin >> a[i];
	for(ll x : a) for(ll y : a) for(ll z : a) ans = max(ans, x|y|z);
	cout << ans << "\n";

	return 0;
}