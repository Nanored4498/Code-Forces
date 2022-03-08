#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		--n;
		int x, y, cx, cy, cxy;
		cin >> x >> y >> cx;
		cy = cxy = cx;
		cout << cxy << '\n';
		while(n--) {
			int l, r, c;
			cin >> l >> r >> c;
			if(l == x) cx = min(cx, c);
			else if(l < x) {
				x = l;
				cx = c;
				cxy = 2e9+1;
			}
			if(r == y) cy = min(cy, c);
			else if(r > y) {
				y = r;
				cy = c;
				cxy = 2e9+1;
			}
			if(l == x && r == y) cxy = min(cxy, c);
			cout << min(cxy, cx+cy) << '\n';
		}
	}

	return 0;
}