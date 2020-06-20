#include <iostream>
#include <vector>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, m;
	cin >> t;
	while(t--) {
		cin >> n >> m;
		vector<string> A(n);
		for(int i = 0; i < n; ++i) cin >> A[i];
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j)
				if(A[i][j] == 'B') {
					if(i > 0 && A[i-1][j] == '.') A[i-1][j] = '#'; 
					if(i+1 < n && A[i+1][j] == '.') A[i+1][j] = '#'; 
					if(j > 0 && A[i][j-1] == '.') A[i][j-1] = '#'; 
					if(j+1 < m && A[i][j+1] == '.') A[i][j+1] = '#'; 
				}
		vector<pii> st;
		vector<vector<bool>> seen(n, vector<bool>(m, false));
		if(A[n-1][m-1] != '#') {
			st.push_back({n-1, m-1});
			seen[n-1][m-1] = true;
		}
		while(!st.empty()) {
			int i = st.back().first, j = st.back().second;
			st.pop_back();
			vector<pii> neighb;
			if(i > 0) neighb.emplace_back(i-1, j);
			if(i+1 < n) neighb.emplace_back(i+1, j);
			if(j > 0) neighb.emplace_back(i, j-1);
			if(j+1 < m) neighb.emplace_back(i, j+1);
			for(const pii &q : neighb) if(A[q.first][q.second] != '#' && !seen[q.first][q.second]) {
				st.push_back(q);
				seen[q.first][q.second] = true;
			}
		}
		bool ok = true;
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j)
				if((A[i][j] == 'G' && !seen[i][j]) || (A[i][j] == 'B' && seen[i][j]))
					ok = false;
		if(ok) cout << "Yes\n";
		else cout << "No\n";
	}

	return 0;
}