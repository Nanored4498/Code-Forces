#include <iostream>

using namespace std;
typedef long long ll;

int main() {
	int t; cin >> t;
	while(t--) {
		ll a = 1;
		ll x = 0;
		for(int i = 0; i < 29; ++i) {
			a <<= 1;
			cout << "? " << a-x << ' ' << 2*a-x << endl;
			ll b; cin >> b;
			if(b != a) x += a>>1;
			cerr << x << endl;
		}
		cout << "? " << 1 << ' ' << x+(1<<29)+2 << endl;
		ll b; cin >> b;
		if(b == x+(1<<29)+1) x += 1<<29;
		cout << "! " << x << endl;
	}
	return 0;
}