#include <iostream>

using namespace std;
using ll = long long;

int main() {
	int n;
	ll m;
	cin >> n >> m;
	const ll MQ = 3981;
	for(ll q = 2; q <= MQ; ++q) {
		ll qn = 1;
		for(int i = 0; i < n-1; ++i) {
			if(qn > m/q) return cout << "impossible" << endl, 0;
			qn *= q;
		}
		for(ll p = q-1; p > 0; --p) {
			ll d = 0, qi = qn, pi = 1;
			for(int i = 0; i < n && d <= m; ++i) {
				d += qi*pi;
				qi /= q;
				pi *= p;
			}
			if((m%d) == 0) return cout << q-p << ' ' << q << endl, 0;
		}
	}
	cout << "impossible" << endl;
	return 0;
}