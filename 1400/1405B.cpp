#include <iostream>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n;
	cin >> t;
	while(t--) {
		cin >> n;
		ll s = 0, r = 0;
		while(n--) {
			ll a;
			cin >> a;
			if(a < 0) {
				ll x = min(s, -a);
				r += -a - x;
				s -= x;	
			} else s += a;
		}
		cout << r << "\n";
	}

	return 0;
}