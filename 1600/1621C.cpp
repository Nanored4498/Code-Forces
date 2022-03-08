#include <iostream>
#include <vector>

using namespace std;

int main() {
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<int> p(n+1, -1);
		for(int i = 1; i <= n; ++i) if(p[i] == -1) {
			cout << "? " << i << endl;
			int x, y;
			cin >> x;
			y = x;
			do {
				cout << "? " << i << endl;
				int z;
				cin >> z;
				if(z == 0) return 0;
				p[y] = z;
				y = z;
			} while(y != x);
		}
		cout << "!";
		for(int i = 1; i <= n; ++i) cout << ' ' << p[i];
		cout << endl;
	}

	return 0;
}