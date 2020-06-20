#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vector<vi> G;
vector<ll> a;
vi b, c, d;
vector<pii> r;

void dfs0(int i, int di) {
	d[i] = di;
	++ di;
	int k = 0;
	while(k < G[i].size()) {
		int j = G[i][k];
		if(d[j] < 0) {
			dfs0(j, di);
			++ k;
		} else {
			G[i][k] = G[i].back();
			G[i].pop_back();
		}
	}
}

void dfs(int i) {
	r[i] = {0, 0};
	if(b[i] < c[i]) ++r[i].first;
	else if(c[i] < b[i]) ++r[i].second;
	for(int j : G[i]) {
		if(r[j].first < 0) dfs(j);
		r[i].first += r[j].first;
		r[i].second += r[j].second;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	a.resize(n+1);
	b.resize(n+1); c.resize(n+1);
	G.resize(n+1);
	r.assign(n+1, {-1, -1});
	d.assign(n+1, -1);
	for(int i = 1; i <= n; ++i) cin >> a[i] >> b[i] >> c[i];
	for(int i = 1; i < n; ++i) {
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	if(accumulate(b.begin(), b.end(), 0) != accumulate(c.begin(), c.end(), 0))
		return cout << "-1\n", 0;
	dfs0(1, 0);
	vi order(n);
	for(int i = 0; i < n; ++i) order[i] = i+1;
	sort(order.begin(), order.end(), [&](int i, int j) {
		return (a[i] < a[j]) || (a[i] == a[j] && d[i] < d[j]) ;
	});
	ll res = 0;
	for(int i : order) {
		if(r[i].first < 0) {
			dfs(i);
			int m = min(r[i].first, r[i].second);
			r[i].first -= m;
			r[i].second -= m;
			res += 2*m*a[i];
		}
	}
	cout << res << endl;

	return 0;
}
