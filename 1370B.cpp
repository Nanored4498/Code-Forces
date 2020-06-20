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
		vi a(2*n+1);
		for(int i = 1; i <= 2*n; ++i) cin >> a[i];
		int m = n-1;
		int x = -1;
		for(int i = 1; i <= 2*n; ++i) if(a[i]&1) {
			if(x == -1) x = i;
			else {
				cout << x << " " << i << "\n";
				x = -1;
				if(--m == 0) break;
			}
		}
		if(m == 0) continue;
		x = -1;
		for(int i = 1; i <= 2*n; ++i) if((a[i]&1) == 0) {
			if(x == -1) x = i;
			else {
				cout << x << " " << i << "\n";
				x = -1;
				if(--m == 0) break;
			}
		}
	}

	return 0;
}