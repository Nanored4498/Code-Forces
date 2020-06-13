#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<ll> vl;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<vi> G(n+1);
	while(m --> 0) {
		int a, b;
		cin >> a >> b;
		G[a].push_back(b);
		G[b].push_back(a);
	}
	vi t(n+1), ans(n);
	for(int i = 1; i <= n; ++i) {
		cin >> t[i];
		ans[i-1] = i;
	}
	bool ok = true;
	for(int i = 1; i <= n; ++i) {
		int d = G[i].size();
		vi p(d);
		for(int j = 0; j < d; ++j) p[j] = t[G[i][j]];
		sort(p.begin(), p.end());
		int m = 1;
		int j = 0;
		while(j < d && p[j] == m) {
			while(j < d && p[j] == m) ++j;
			++ m;
		}
		if(m != t[i]) {
			ok = false;
			break;
		}
	}
	if(ok) {
		sort(ans.begin(), ans.end(), [&](int i, int j) {
			return t[i] < t[j];
		});
		for(int i = 0; i < n; ++i) cout << ans[i] << " \n"[i+1 == n];
	} else cout << "-1\n";

	return 0;
}