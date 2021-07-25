#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef pair<int, int> pii;
const int MOD = 998244353;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<vi> m(2*n, vi(n)), c(n, vi(n, 0));
		for(vi &r : m) {
			for(int &x : r) cin >> x;
			for(int i = 0; i < n; ++i) ++ c[i][--r[i]];
		}

		vector<pii> Q;
		for(int j = 0; j < n; ++j)
			for(int i = 0; i < n; ++i)
				if(c[j][i] == 1)
					Q.emplace_back(j, i);
		vi rs(2*n), g;
		for(int i = 0; i < 2*n; ++i) rs[i] = i;
		int ans = 1;
		while(!rs.empty()) {
			while(!Q.empty() && c[Q.back().first][Q.back().second] != 1) Q.pop_back();
			int r;
			if(Q.empty()) {
				r = rs[0];
				ans = (ans + ans) % MOD;
			} else {
				auto [j, k] = Q.back();
				Q.pop_back();
				for(int i : rs) if(m[i][j] == k) {
					r = i;
					break;
				}
			}
			g.push_back(r+1);
			for(int i = 0; i < rs.size(); ++i) {
				int k = rs[i];
				for(int j = 0; j < n; ++j) if(m[k][j] == m[r][j]) {
					for(int l = 0; l < n; ++l)
						if(--c[l][m[k][l]] == 1)
							Q.emplace_back(l, m[k][l]);
					swap(rs[i], rs.back());
					rs.pop_back();
					-- i;
					break;
				}
			}
		}
		
		cout << ans << '\n';
		for(int i : g) cout << i << ' ';
		cout << '\n';
	}

	return 0;
}