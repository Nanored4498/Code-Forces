#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vi w(n+1), x(m+1), y(m+1), ans;
	vector<vi> S(n+1);
	vector<bool> used(m+1, false);
	for(int i = 1; i <= n; ++i) cin >> w[i];
	for(int i = 1; i <= m; ++i) {
		cin >> x[i] >> y[i];
		S[x[i]].push_back(i);
		S[y[i]].push_back(i);
	}
	priority_queue<pii> Q;
	for(int i = 1; i <= n; ++i) {
		w[i] -= S[i].size();
		Q.emplace(w[i], i);
	}
	while(!Q.empty()) {
		int v = Q.top().first, i = Q.top().second;
		Q.pop();
		if(v != w[i]) continue;
		if(v < 0) return cout << "DEAD\n", 0;
		set<int> upd;
		for(int j : S[i]) if(!used[j]) {
			ans.push_back(j);
			used[j] = true;
			++ w[x[j]];
			++ w[y[j]];
			if(x[j] != i) upd.insert(x[j]);
			if(y[j] != i) upd.insert(y[j]);
		}
		for(int j : upd) Q.emplace(w[j], j);
	}
	cout << "ALIVE\n";
	for(int i = m-1; i >= 0; --i) cout << ans[i] << " \n"[i==0];

	return 0;
}