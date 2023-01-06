#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	vector<vi> c(n, vi(n, 0));
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			int cij; cin >> cij;
			++ c[i][cij-1];
		}
	}
	vector<vi> ans;
	while(true) {
		int i0 = 0;
		for(; i0 < n; ++i0) {
			bool b = false;
			for(int j = 0; j < n; ++j) b |= c[i0][j] >= 2;
			if(b) break;
		}
		if(i0 == n) break;
		vi d(n);
		int i = i0;
		do {
			int j = 0;
			while(j < n && c[i][j] < 2) ++j;
			if(j == n) d[i] = d[i?i-1:n-1];
			else d[i] = j;
			i = (i+1)%n;
		} while(i != i0);
		for(int i = 0; i < n; ++i) {
			-- c[i][d[i]];
			++ c[(i+1)%n][d[i]];
		}
		ans.emplace_back(move(d));
	}
	cout << ans.size() + (n*n-n)/2 << '\n';
	for(const vi &d : ans) {
		for(int x : d) cout << x+1 << ' ';
		cout << '\n';
	}
	for(int i = 1; i < n; ++i) for(int j = i; j--;) {
		for(int k = 0; k < n; ++k) cout << ((k+n-i)%n)+1 << ' ';
		cout << '\n';
	}

	return 0;
}