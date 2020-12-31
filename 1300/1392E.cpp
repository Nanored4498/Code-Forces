#include <iostream>

using namespace std;
typedef long long ll;

int main() {
	int n, q;
	cin >> n;
	for(int i = 0; i < n; ++i) {
		if(~i&1) for(int j = 0; j < n; ++j) cout << "0 ";
		else for(int j = 0; j < n; ++j) cout << (1LL << (i+j)) << " ";
		cout << "\n";
	}
	cin >> q;
	while(q--) {
		ll k;
		cin >> k;
		int x = 1, y = 1;
		while(k > 0) {
			k >>= 1;
			cout << x << " " << y << "\n";
			while(~k&1) {
				k >>= 1;
				++ y;
				cout << x << " " << y << "\n";
			}
			++ x;
			k >>= 1;
			cout << x << " " << y << "\n";
			while(k&1) {
				k >>= 1;
				++ y;
				cout << x << " " << y << "\n";
			}
			++ x;
		}
		if(x == n) while(y <= n) cout << x << " " << (y++) << "\n";
		cout << flush;		
	}
}