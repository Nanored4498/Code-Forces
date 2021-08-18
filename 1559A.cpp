#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		int x;
		cin >> x;
		while(--n>0) {
			int y;
			cin >> y;
			x &= y;
		}
		cout << x << endl;
	}

	return 0;
}