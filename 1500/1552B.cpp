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
		vector<vi> r(n, vi(5));
		for(vi &x : r) for(int &y : x) cin >> y;
		int g = 0;
		for(int i = 1; i < n; ++i) {
			int c = 0;
			for(int j = 0; j < 5; ++j) if(r[i][j] < r[g][j]) ++c;
			if(c >= 3) g = i;
		}
		for(int i = 0; i < n; ++i) if(i != g) {
			int c = 0;
			for(int j = 0; j < 5; ++j) if(r[i][j] < r[g][j]) ++c;
			if(c >= 3) {
				g = -2;
				break;
			}
		}
		cout << ++g << '\n';
	}

	return 0;
}