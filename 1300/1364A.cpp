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
		int n, x, a, s=0, l=-1, r=-1;
		cin >> n >> x;
		for(int i = 0; i < n; ++i) {
			cin >> a;
			s = (s+a)%x;
			if(a%x != 0) {
				if(l < 0) l = i;
				r = i;
			}
		}
		if(s != 0) cout << n;
		else if(l < 0) cout << -1;
		else cout << n - min(l+1, n-r);
		cout << "\n";
	}

	return 0;
}