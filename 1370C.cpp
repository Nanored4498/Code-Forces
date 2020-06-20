#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		if(n == 1) cout << "FastestFinger\n";
		else if(n == 2) cout << "Ashishgup\n";
		else if(n&1) cout << "Ashishgup\n";
		else if(n&2) {
			n >>= 1;
			bool prime = true;
			for(int d = 3; d*d <= n; ++d)
				if(n % d == 0) {
					prime = false;
					break;
				}
			if(prime) cout << "FastestFinger\n";
			else cout << "Ashishgup\n";
		} else {
			while(n%2 == 0) n >>= 1;
			if(n == 1) cout << "FastestFinger\n";
			else cout << "Ashishgup\n";
		}
	}

	return 0;
}