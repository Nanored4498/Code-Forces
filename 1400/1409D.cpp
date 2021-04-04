#include <iostream>

using namespace std;
typedef long long ll;

int ds[20];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		ll n;
		int s;
		cin >> n >> s;
		ll ans = 0;
		int r = 0, p = 0;
		while(n > 0) {
			ds[p] = n % 10;
			r += ds[p++];
			n /= 10;
		}
		ds[p] = 0;
		p = 0;
		ll p10 = 1;
		while(r > s) {
			r -= ds[p];
			ans += (10 - ds[p]) * p10;
			while(true) {
				++ ds[++p];
				p10 *= 10;
				if(ds[p] == 10) {
					r -= 9;
				} else {
					++ r;
					break;
				}
			}
		}
		cout << ans << "\n";
	}

	return 0;
}