#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, k;
	cin >> n >> m >> k;

	if(k&1) {
		for(int i = 0; i < n; ++i) {
			for(int j = 0; j < m; ++j) cout << "-1 ";
			cout << '\n';
		}
		return 0;
	}

	vector<vi> ex(n, vi(m-1)), ey(n-1, vi(m)), ans(n, vi(m, 0)), tmp(n, vi(m));
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < m-1; ++j)
			cin >> ex[i][j];
	for(int i = 0; i < n-1; ++i)
		for(int j = 0; j < m; ++j)
			cin >> ey[i][j];

	k /= 2;
	while(k--) {
		for(int i = 0; i < n; ++i) {
			for(int j = 0; j < m; ++j) {
				tmp[i][j] = INT32_MAX;
				if(i > 0) tmp[i][j] = min(tmp[i][j], ans[i-1][j] + ey[i-1][j]);
				if(i+1 < n) tmp[i][j] = min(tmp[i][j], ans[i+1][j] + ey[i][j]);
				if(j > 0) tmp[i][j] = min(tmp[i][j], ans[i][j-1] + ex[i][j-1]);
				if(j+1 < m) tmp[i][j] = min(tmp[i][j], ans[i][j+1] + ex[i][j]);
			}
		}
		swap(ans, tmp);
	}
	for(int i = 0; i < n; ++i) {
		for(int a : ans[i]) cout << 2*a << ' ';
		cout << '\n';
	}

	return 0;
}