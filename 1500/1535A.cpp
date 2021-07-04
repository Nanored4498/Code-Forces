#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int a[4], b[4];
		for(int i = 0; i < 4; ++i) cin >> a[i];
		for(int i = 0; i < 4; ++i) b[i] = i;
		sort(b, b+4, [&](int i, int j) { return a[i] > a[j]; });
		if((b[0]&2) == (b[1]&2)) cout << "NO";
		else cout << "YES";
		cout << '\n';
	}

	return 0;
}