#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {

	int n;
	cin >> n;
	int x = 1, y = -1, xy = 0xfff, xz;
	for(int z = 2; z <= n; ++z) {
		cout << "? " << x << " " << z << endl;
		cin >> xz;
		if(xz < 0) return 0;
		bool ny = (xy|xz) > xz, nz = (xy|xz) > xy;
		if(nz) {
			if(ny) {
				xy &= xz;
				y = -1;
			}
		} else if(y < 0 || ny) {
			y = z;
			xy = xz;
		} else {
			x = z;
			cout << "? " << x << " " << y << endl;
			cin >> xy;
			if(xy < 0) return 0;
		}
	}
	cerr << x << " " << y << " " << xy << endl;
	vi ans(n+1), inv(n, -1);
	bool swaped = false;
	for(int z = 1; z <= n; ++z) {
		if(z == x || z == y) {
			ans[z] = xy;
			continue;
		} else {
			cout << "? " << x << " " << z << endl;
			cin >> ans[z];
			if(ans[z] < 0) return 0;
		}
		if(ans[z] > n || inv[ans[z]] >= 0) {
			if(!swaped) {
				x = y;
				swaped = true;
				--z;
				continue;
			}
			ans[inv[ans[z]]] ^= xy;
		}
		inv[ans[z]] = z;
	}
	ans[x] = 0;
	cout << "!";
	for(int i = 1; i <= n; ++i) cout << " " << ans[i];
	cout << endl;

	return 0;
}