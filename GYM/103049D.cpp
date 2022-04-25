#include <iostream>
#include <cmath>

using namespace std;
typedef long long ll;

int main() {
	int n; cin >> n;
	++n;
	while(--n) {
		cout << "0 0" << endl;
		ll d0; cin >> d0;
		if(!d0) continue;
		for(int i = 1; i <= n; ++i) {
			cout << "0 " << i << endl;
			ll d; cin >> d;
			swap(d, d0);
			ll y2 = d-d0-1 + 2*i;
			if(y2&1) continue;
			ll y = y2/2;
			ll x2 = d0 - (y-i)*(y-i);
			ll x = sqrt(x2);
			while(x*x < x2) ++x;
			while(x*x > x2) --x;
			if(x*x != x2) continue;
			cout << x << ' ' << y << endl;
			cin >> d;
			if(!d) break;
			if(i == n) return 1;
		}
	}

	return 0;
}