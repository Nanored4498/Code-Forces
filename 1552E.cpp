#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vi c(n*k), ni(n*k, 0);
	for(int &x : c) cin >> x;
	int m = (n+k-2) / (k-1);
	vector<bool> ok(n, false);
	vector<int> last(n, -1);
	vector<pair<int, int>> ans(n);
	for(int i = 0; i < n*k; ++i) {
		int v = --c[i];
		if(ok[v]) continue;
		int &j = last[v];
		if(j != -1) {
			bool b = true;
			for(int l = j; l <= i; ++l)
				if(ni[l] == m) b = false;
			if(b) {
				for(int l = j; l <= i; ++l) ++ ni[l];
				ans[v] = { j+1, i+1 };
				ok[v] = true;
			}
		}
		j = i;
	}
	for(auto [i, j] : ans)
		cout << i << ' ' << j << '\n';

	return 0;
}