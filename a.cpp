#include <iostream>
#include <vector>
#include <algorithm>

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
		bool neg = false, pos = false;
		for(int i = 0; i < n; ++i) {
			int x;
			cin >> x;
			if(x < 0) neg = true;
			if(x < 0) pos = true;
		}
		if(neg && pos) cout << "NO\n";
		else {
			cout << "YES\n101\n";
			if(neg) for(int i = -100; i <= 0; ++i) cout << i << ' ';
			else for(int i = 0; i <= 100; ++i) cout << i << ' ';
			cout << '\n';
		}
	}

	return 0;
}