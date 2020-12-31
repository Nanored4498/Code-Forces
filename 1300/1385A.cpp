#include <iostream>

using namespace std;

int main() {
	int t, x, y, z;
	cin >> t;
	while(t--) {
		cin >> x >> y >> z;
		if(x == y && x >= z) cout << "YES\n" << x << " " << z << " " << z << "\n";
		else if(y == z && y >= x) cout << "YES\n" << x << " " << x << " " << y << "\n";
		else if(x == z && x >= y) cout << "YES\n" << y << " " << x << " " << y << "\n";
		else cout << "NO\n";
	}
	return 0;
}