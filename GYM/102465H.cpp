#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int N, E;
vector<vector<pii>> G;

vi dijkstra(int i0) {
	vi ds(N, INT32_MAX);
	ds[i0] = 0;
	priority_queue<pii, vector<pii>, greater<pii>> q;
	q.emplace(0, i0);
	while(!q.empty()) {
		int d = q.top().first, i = q.top().second;
		q.pop();
		if(d > ds[i]) continue;
		for(const pii &p : G[i]) {
			int j = p.first, dj = d + p.second;
			if(ds[j] > dj) {
				ds[j] = dj;
				q.emplace(dj, j);
			}
		}
	}
	return ds;
}

vi mi;
void set(int y, int z) {
	while(y < mi.size()) {
		mi[y] = min(mi[y], z);
		y += (y)&(-y);
	}
}
int get(int y) {
	int ans = INT32_MAX;
	while(y > 0) {
		ans = min(ans, mi[y]);
		y -= (y)&(-y);
	}
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> N >> E;
	G.resize(N);
	int a, b, w;
	while(E --> 0) {
		cin >> a >> b >> w;
		G[a].emplace_back(b, w);
		G[b].emplace_back(a, w);
	}
	vi d[3];
	for(int i = 0; i < 3; ++i) d[i] = dijkstra(i);
	for(int i = 0; i < N; ++i) ++ d[1][i];
	vi order(N);
	for(int i = 0; i < N; ++i) order[i] = i;
	sort(order.begin(), order.end(), [&](int i, int j) {
		if(d[0][i] != d[0][j]) return d[0][i] < d[0][j];
		if(d[1][i] != d[1][j]) return d[1][i] < d[1][j];
		return d[2][i] < d[2][j];
	});
	int ans = 0, d2m = 0;
	for(int i = 0; i < N; ++i) d2m = max(d2m, d[1][i]);
	mi.assign(d2m+1, INT32_MAX);
	for(int i = 0; i < N; ++i) {
		int j = order[i];
		int y = d[1][j], z = d[2][j];
		int mz = get(y);
		if(z < mz) {
			++ ans;
			set(y, z);
			while(i+1 < N && d[0][j] == d[0][order[i+1]] && y == d[1][order[i+1]] && z == d[2][order[i+1]]) {
				++ ans;
				++ i;
			}
		}
	}
	cout << ans << "\n";

	return 0;
}