#include <iostream>
#include <vector>
#include <queue>

using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;
const ll INF = 1e13;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<vector<ll>> d(n, vector<ll>(n, INF));
	for(int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		cin >> d[a][b];
	}
	for(int i = 0; i < n; ++i) {
		int j0 = 0;
		for(int j = 1; j < n; ++j) if(d[i][j] < d[i][j0]) j0 = j;
		for(int j = (j0+1)%n; j != j0; j = (j+1)%n) d[i][j] = min(d[i][j], 1+d[i][(j+n-1)%n]);
	}
	for(int i = 0; i < n; ++i) {
		vector<ll> di = d[i];
		di[i] = 0;
		priority_queue<pli, vector<pli>, greater<pli>> Q;
		for(int j = 0; j < n; ++j) if(j != i) Q.emplace(di[j], j);
		while(!Q.empty()) {
			auto [dij, j] = Q.top();
			Q.pop();
			if(dij > di[j]) continue;
			int add = dij % n;
			ll d2;
			for(int k = 0; k < n; ++k)
				if((d2 = dij + d[j][k]) < di[(k+add)%n]) {
					di[(k+add)%n] = d2;
					Q.emplace(d2, (k+add)%n);
				}
		}
		for(int j = 0; j < n; ++j) cout << di[j] << ' ';
		cout << '\n';
	}

	return 0;
}