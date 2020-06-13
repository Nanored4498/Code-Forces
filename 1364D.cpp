#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
const int MAX_N = 1e5+5;

int n, m, k;
vi Set[2];
int parent[MAX_N];
vi G[MAX_N];

bool dfs(int u, int d) {
	Set[d&1].push_back(u);
	for(int v : G[u]) if(v != parent[u]) {
		if(parent[v] < 0) {
			if(Set[0].size()+Set[1].size() >= k) continue;
			parent[v] = u;
			if(dfs(v, d+1)) return true;
		} else {
			vi a, b;
			do {
				a.push_back(u);
				u = parent[u];
			} while(parent[a.back()] != a.back());
			do {
				b.push_back(v);
				v = parent[v];
			} while(parent[b.back()] != b.back());
			int p;
			while(!b.empty() && b.back() == a.back()) {
				p = b.back();
				a.pop_back();
				b.pop_back();
			}
			cout << "2\n" << 1+a.size()+b.size() << "\n" << p;
			for(int i = a.size()-1; i >= 0; --i) cout << " " << a[i];
			for(int i = 0; i < b.size(); ++i) cout << " " << b[i];
			cout << "\n";
			return true;
		}
	}
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m >> k;
	while(m--) {
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	for(int u = 1; u <= n; ++u) parent[u] = -1;
	for(int u = 1; u <= n; ++u) if(parent[u] < 0) {
		parent[u] = u;
		if(dfs(u, 0)) return 0;
		if(Set[0].size() + Set[1].size() >= k) break;
	}
	int d = Set[0].size() >= (k+1)/2 ? 0 : 1;
	cout << "1\n";
	for(int i = 0; i < (k+1)/2; ++i) cout << Set[d][i] << " \n"[i+1==(k+1)/2];

	return 0;
}