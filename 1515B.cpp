#include <iostream>

using namespace std;
typedef unsigned int uint;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		uint n;
		cin >> n;
		bool yes = false;
		uint i = 1;
		for(; 2u*i*i < n; ++i) {
			if(4u*i*i == n) {
				yes = true;
				break;
			}
		}
		yes |= 2u*i*i == n;
		if(yes) cout << "YES\n";
		else cout << "NO\n";
	}

	return 0;
}