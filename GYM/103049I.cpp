#include <iostream>

using namespace std;
const int MN = 405;

int n;
int d[MN], t[3][MN], s[3][MN][MN];
int l[3][3][MN];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n;
	for(int i = 0; i < n; ++i) cin >> d[i];
	for(int j : {0, 1, 2}) {
		for(int i = 0; i < n; ++i) cin >> t[j][i];
		for(int i = 0; i < n; ++i) {
			s[j][i][i] = 0;
			for(int k = 1; k < n; ++k) {
				int x = (i+k-1)%n, y = (i+k)%n;
				s[j][i][y] = s[j][i][x] + t[j][x] + d[x];
			}
		}
	}
	for(int i : {0, 1, 2}) for(int j : {0, 1, 2}) if(i != j) {
		for(int x = 0; x < n; ++x) {
			for(l[i][j][x] = 1; l[i][j][x] < n; ++l[i][j][x]) {
				int y = (x + l[i][j][x]) % n;
				bool good = true;
				for(int z = 0; z < n; ++z) if(s[i][x][z]+t[i][z] > s[j][y][z] && s[i][x][z] < s[j][y][z]+t[j][z]) {
					good = false;
					break;
				}
				if(good) break;
			}
		}
	}
	for(int x = 0; x < n; ++x) {
		int y = (x+l[0][1][x])%n;
		int z = (y+l[1][2][y])%n;
		int tot = l[0][1][x] + l[1][2][y] + l[2][0][z];
		if(tot <= n) {
			cout << x+1 << ' ' << y+1 << ' ' << z+1 << endl;
			return 0;
		}
		y = (x+l[0][2][x])%n;
		z = (y+l[2][1][y])%n;
		tot = l[0][2][x] + l[2][1][y] + l[1][0][z];
		if(tot <= n) {
			cout << x+1 << ' ' << z+1 << ' ' << y+1 << endl;
			return 0;
		}
	}
	cout << "impossible" << endl;

	return 0;
}