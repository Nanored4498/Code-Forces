#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		bool a = false, b = false;
		while(n--) {
			int l, r;
			cin >> l >> r;
			if(l == k) a = true;
			if(r == k) b = true;
		}
		cout << (a && b ? "YES" : "NO") << '\n';
	}

	return 0;
}