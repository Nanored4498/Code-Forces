#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<vi> p(n);
	vi f(n, 0);
	for(vi &v : p) {
		int c; cin >> c;
		v.resize(c);
		for(int &x : v) {
			cin >> x;
			--x;
			++ f[x];
		}
	}

	int x = -1;
	for(int i = 0; i < n; ++i) if(!f[i]) {
		if(x >= 0) {
			cout << 0 << endl;
			return 0;
		}
		x = i;
	}
	if(x < 0) {
		cout << 0 << endl;
		return 0;
	}
	for(int i = 1; i < n; ++i) {
		int y = -1;
		for(int z : p[x]) {
			--f[z];
			if(!f[z]) {
				if(y != -1) {
					cout << 0 << endl;
					return 0;
				}
				y = z;
			}
		}
		if(y == -1) {
			cout << 0 << endl;
			return 0;
		}
		x = y;
	}
	cout << 1 << endl;


	return 0;
}