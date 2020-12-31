#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, s=0;
		cin >> n;
		vi a(n+1);
		for(int i = 1; i <= n; ++i) {
			cin >> a[i];
			s += a[i];
		}
		if(s % n == 0) {
			int m = s / n;
			cout << 3*(n-1) << "\n";
			for(int i = 2; i <= n; ++i) {
				int x = a[i] % i;
				x = (i - x) % i;
				cout << "1 " << i << " " << x << "\n";
				x = (a[i] + x) / i;
				cout << i << " 1 " << x << "\n";
			}
			for(int i = 2; i <= n; ++i) cout << "1 " << i << " " << m << "\n";
		} else cout << "-1\n";
	}

	return 0;
}