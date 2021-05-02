#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<vi> ans(n);
	for(int i = 0; i < n; ++i) {
		ans[i].resize(i+1, 0);
		cin >> ans[i][i];
	}
	for(int i = 0; i < n; ++i) {
		int x = i, y = i;
		int p = ans[i][i]-1;
		while(p--) {
			if(y > 0 && ans[x][y-1] == 0) -- y;
			else ++ x;
			ans[x][y] = ans[i][i];
		}
	}
	for(int i = 0; i < n; ++i) {
		for(int a : ans[i]) cout << a << ' ';
		cout << '\n';
	}

	return 0;
}