#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		vector<pair<vi, int>> a(n, { vi(m), 0 });
		for(auto &r : a) for(int &x : r.first) cin >> x;
		for(int i = 0; i < n; ++i) a[i].second = i;
		sort(a.begin(), a.end(), [&](const auto &x, const auto &y) { return x.first[0] < y.first[0]; });

		// blue
		vector<vi> blueR(n), blueL(n);
		for(int i = 0; i < n; ++i) {
			blueR[i] = a[i].first;
			blueL[i] = a[i].first;
			for(int j = 1; j < m; ++j) blueR[i][j] = max(blueR[i][j], blueR[i][j-1]);
			for(int j = m-2; j >= 0; --j) blueL[i][j] = min(blueL[i][j], blueL[i][j+1]);
			if(i != 0) for(int j = 0; j < m; ++j) {
				blueR[i][j] = max(blueR[i][j], blueR[i-1][j]);
				blueL[i][j] = min(blueL[i][j], blueL[i-1][j]);
			}
		}

		// red
		vector<vi> redR(n), redL(n);
		for(int i = n-1; i >= 0; --i) {
			redR[i] = a[i].first;
			redL[i] = a[i].first;
			for(int j = 1; j < m; ++j) redR[i][j] = min(redR[i][j], redR[i][j-1]);
			for(int j = m-2; j >= 0; --j) redL[i][j] = max(redL[i][j], redL[i][j+1]);
			if(i+1 != n) for(int j = 0; j < m; ++j) {
				redR[i][j] = min(redR[i][j], redR[i+1][j]);
				redL[i][j] = max(redL[i][j], redL[i+1][j]);
			}
		}

		// test
		bool found = false;
		for(int i = 1; i < n; ++i) for(int j = 1; j < m; ++j) {
			if(blueR[i-1][j-1] >= redR[i][j-1]) continue;
			if(blueL[i-1][j] <= redL[i][j]) continue;
			found = true;
			string s(n, 'R');
			for(int k = 0; k < i; ++k) s[a[k].second] = 'B';
			cout << "YES\n" << s << ' ' << j << '\n';
			break;
		}
		if(!found) cout << "NO\n";
	}

	return 0;
}