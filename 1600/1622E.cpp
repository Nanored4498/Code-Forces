#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		vector<int> x(n), ans;
		for(int &a : x) cin >> a;
		vector<string> s(n);
		for(string &a : s) cin >> a;
		int best = -1;
		for(int p = 0; p < (1<<n); ++p) {
			vector<int> c(2*n+1, 0), y(n+1, 0), t(m);
			for(int j = 0; j < m; ++j) {
				int a = n;
				for(int i = 0; i < n; ++i) if(s[i][j]=='1') a += 2*((p>>i)&1)-1;
				++ c[a];
			}
			for(int i = 1; i <= 2*n; ++i) c[i] += c[i-1];
			for(int j = 0; j < m; ++j) {
				int a = n;
				for(int i = 0; i < n; ++i) if(s[i][j]=='1') a += 2*((p>>i)&1)-1;
				for(int i = 0; i < n; ++i) if(s[i][j]=='1') y[i] += c[a];
				t[j] = c[a];
				-- c[a];
			}
			int score = 0;
			for(int i = 0; i < n; ++i) score += abs(x[i] - y[i]);
			if(score > best) {
				best = score;
				ans = t;
			}
		}
		for(int a : ans) cout << a << ' '; cout << '\n';
	}

	return 0;
}