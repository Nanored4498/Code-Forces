#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<ll> vl;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	int m = (n+1)/2;
	vl a(m);
	for(int i = 0; i < m; ++i) cin >> a[i];
	ll x;
	cin >> x;
	ll s = (n-m)*x;
	if(x > 0) {
		for(int i = 0; i < m; ++i) s += a[i];
		if(s > 0) cout << n << "\n";
		else cout << "-1\n";
	} else {
		int i = m-1, j = n;
		while(i >= 0) {
			s += a[i];
			if(s <= 0) {
				s += (n-j)*x;
				j = n;
			} else {
				-- j;
				s -= x;
			}
			-- i;
		}
		if(s <= 0) cout << "-1\n";
		else cout << j-i << "\n";
	}

	return 0;
}