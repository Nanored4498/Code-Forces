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
		vector<ll> a(n);
		for(ll &x : a) cin >> x;
		ll x;
		cin >> x;
		for(ll &y : a) y -= x;
		vector<int> d(n);
		d[0] = 1;
		for(int i = 1, p = 0; i < n; ++i) {
			if(a[i]+a[i-1] < 0) p = i;
			else if(i > 1 && a[i]+a[i-1]+a[i-2] < 0) p = i-1;
			d[i] = max(d[i-1], i-p+1 + (p > 1 ? d[p-2] : 0));
		}
		cout << d[n-1] << '\n';
	}

	return 0;
}