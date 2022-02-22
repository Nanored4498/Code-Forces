#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m, k;
		cin >> n >> m >> k;
		int p = 0, q = n/m, r = n%m, r2 = m-r;
		for(int i = 0; i < k; ++i) {
			for(int j = 0; j < r; ++j) {
				cout << q+1;
				for(int x = 0; x < q+1; ++x) cout << ' ' << ((p+j*(q+1)+x)%n + 1);
				cout << '\n';
			}
			p += r*(q+1);
			for(int j = 0; j < r2; ++j) {
				cout << q;
				for(int x = 0; x < q; ++x) cout << ' ' << ((p+j*q+x)%n + 1);
				cout << '\n';
			}
		}
		cout << '\n';
	}

	return 0;
}