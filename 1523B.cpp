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
		int n;
		cin >> n;
		vi a(n);
		for(int &x: a) cin >> x;
		cout << 3*n << '\n';
		for(int i = 1; i < n; i += 2) {
			cout << "2 " << i << ' ' << i+1 << '\n'; a[i] -= a[i-1];
			cout << "1 " << i << ' ' << i+1 << '\n'; a[i-1] += a[i];
			cout << "1 " << i << ' ' << i+1 << '\n'; a[i-1] += a[i];
			cout << "2 " << i << ' ' << i+1 << '\n'; a[i] -= a[i-1];
			cout << "1 " << i << ' ' << i+1 << '\n'; a[i-1] += a[i];
			cout << "1 " << i << ' ' << i+1 << '\n'; a[i-1] += a[i];
		}
	}

	return 0;
}