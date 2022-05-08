#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		int ans = 0;
		while(n--) {
			int a; cin >> a;
			ans |= a;
		}
		cout << ans << '\n';
	}

	return 0;
}