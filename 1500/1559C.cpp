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
		int n;
		cin >> n;
		vi a(n);
		for(int &x : a) cin >> x;
		if(a == vi(n, 0)) {
			for(int i = 1; i <= n+1; ++i) cout << i << ' ';
		} else if(a == vi(n, 1)) {
			cout << n+1;
			for(int i = 1; i <= n; ++i) cout << ' ' << i;
		} else {
			int j = 1;
			while(a[j-1] == a[j]) ++j;
			if(a[j] == 0) {
				if(a[n-1] == 0) {
					for(int i = j+1; i <= n; ++i) cout << i << ' ';
					cout << n+1;
					for(int i = 1; i <= j; ++i) cout << ' ' << i;
				} else {
					while(a[j] == 0) ++j;
				}
			}
			if(a[j] == 1) {
				for(int i = 1; i <= j; ++i) cout << i << ' ';
				cout << n+1;
				for(int i = j+1; i <= n; ++i) cout << ' ' << i;
			}
		}
		cout << '\n';
	}

	return 0;
}