#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int a, b, n;
		cin >> a >> b >> n;
		int ans = 0;
		while(a <= n && b <= n) {
			if(a < b) swap(a, b);
			b += a;
			++ ans;
		}
		cout << ans << "\n";
	}

	return 0;
}