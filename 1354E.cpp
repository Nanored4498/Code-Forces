#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;

vvi G;
vi c2;
vvi cc2;
int n, m, n1, n2, n3, u, v;

bool col2(int i, int c) {
	c2[i] = c;
	cc2.back()[c] ++;
	c = (c+1)%2;
	for(int j : G[i]) {
		if(c2[j] < 0 && col2(j, c)) return true;
		else if(c2[j] != c) return true;
	}
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m >> n1 >> n2 >> n3;
	G.resize(n);
	for(int i = 0; i < m; ++i) {
		cin >> u >> v;
		--u; --v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	
	c2.assign(n, -1);
	for(int i = 0; i < n; ++i) if(c2[i] < 0) {
		cc2.push_back(vi(2, 0));
		if(col2(i, 0)) {
			cout << "NO\n";
			return 0;
		}
	}

	int clus = cc2.size();
	vector<vector<bool>> dp(clus, vector<bool>(n2+1, false));
	for(int c : cc2[0]) if(c <= n2) dp[0][c] = true;
	for(int i = 1; i < clus; ++i) {
		for(int j = 0; j <= n2; ++j) for(int c : cc2[i]) {
			if(j-c >= 0 && dp[i-1][j-c]) dp[i][j] = true;
		}
	}

	if(dp.back()[n2]) {
		vector<int> took(clus);
		for(int i = clus-1, j = n2; i >= 0; --i) {
			int c = cc2[i][0];
			if((i > 0 && j-c >= 0 && dp[i-1][j-c]) || (i == 0 && j-c == 0)) {
				took[i] = 0;
				j -= c;
			} else {
				took[i] = 1;
				j -= cc2[i][1];
			}
		}
		c2.assign(n, -1);
		cc2.clear();
		int j = 0;
		for(int i = 0; i < n; ++i) if(c2[i] < 0) {
			cc2.push_back(vi(2, 0));
			col2(i, took[j]);
			++ j;
		}
		int c1 = 0;
		cout << "YES\n";
		for(int i = 0; i < n; ++i) {
			if(c2[i] == 0) cout << 2;
			else if(c1 < n1) { cout << 1; ++c1; }
			else cout << 3;
		}
		cout << "\n";
	} else cout << "NO\n";

	return 0;
}