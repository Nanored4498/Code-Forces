#include <iostream>
#include <vector>

using namespace std; 
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, u;
	cin >> n >> m;
	vector<vi> e(n+1);
	vi d(m), c(m);
	for(int i = 0; i < m; ++i) {
		cin >> u >> d[i];
		e[u].push_back(i);
	}
	int k = 1;
	vector<int> seen(n+1, false);
	for(int i = 1; i <= n; ++i) if(!seen[i]) {
		vi st{i};
		while(!st.empty()) {
			int u = st.back();
			seen[u] = 1;
			if(e[u].empty()) {
				seen[u] = 2;
				st.pop_back();
			} else {
				int j = e[u].back(); e[u].pop_back();
				int v = d[j];
				if(seen[v] == 1) c[j] = k = 2;
				else {
					c[j] = 1;
					if(seen[v] == 0) st.push_back(v);
				}
			}
		}
	}
	cout << k << "\n";
	for(int i = 0; i < m; ++i) cout << c[i] << " \n"[i+1==m];

	return 0;
}