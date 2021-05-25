#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, x;
		cin >> n >> x;
		vi w(n);
		for(int i = 0; i < n; ++i) cin >> w[i];
		int s = 0;
		bool imp = false;
		for(int i = 0; i < n; ++i) {
			s += w[i];
			if(s == x) {
				if(i+1 < n) {
					swap(w[i], w[i+1]);
					break;
				} else imp = true;
			}
		}
		if(imp) cout << "NO\n";
		else {
			cout << "YES\n";
			for(int i = 0; i < n; ++i) cout << w[i] << ' ';
			cout << '\n';
		}
	}

	return 0;
}