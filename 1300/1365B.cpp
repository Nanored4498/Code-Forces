#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, x, y;
		cin >> n;
		bool yes = true;
		cin >> x;
		for(int i = 1; i < n; ++i) {
			cin >> y;
			if(y < x) yes = false;
			x = y;
		}
		cin >> x;
		for(int i = 1; i < n; ++i) {
			cin >> y;
			if(y != x) yes = true;;
		}
		if(yes) cout << "Yes\n";
		else cout << "No\n";
	}

	return 0;
}