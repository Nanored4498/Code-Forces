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
		int last = -1;
		int ans = 0;
		for(int i = 0; i < n; ++i) {
			int x;
			cin >> x;
			if(x == last) ++x;
			if(x > last) ++ans;
			last = max(last, x);
		}
		cout << ans << "\n";
	}

	return 0;
}