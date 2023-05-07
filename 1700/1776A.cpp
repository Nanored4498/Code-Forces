#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		int last = 0;
		int count = 0;
		while(n--) {
			int a; cin >> a;
			count += (a - last) / 120;
			last = a;
		}
		count += (1440 - last) / 120;
		cout << (count >= 2 ? "YES" : "NO") << "\n";
	}

	return 0;
}