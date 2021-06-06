#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef pair<ll, ll> pll;

vector<ll> a, c;
vector<vector<int>> ps;

ll A, C, W;
void f(int v) {
	while(A < W) {
		while(!ps[v].empty() && a[ps[v].back()] == 0)
			ps[v].pop_back();
		if(ps[v].empty()) break;
		f(ps[v].back());
	}
	if(A < W) {
		ll x = min(a[v], W - A);
		A += x;
		C += x * c[v];
		a[v] -= x;
	}
}

int main() {
	int q;
	cin >> q;
	a.resize(q+1);
	c.resize(q+1);
	ps.resize(q+1);
	cin >> a[0] >> c[0];

	for(int i = 1; i <= q; ++i) {
		int t;
		cin >> t;
		if(t == 1) {
			int p;
			cin >> p >> a[i] >> c[i];
			ps[i].push_back(p);
			for(int k = 0; k < ps[ps[i][k]].size(); ++k)
				ps[i].push_back(ps[ps[i][k]][k]);
		} else {
			int v;
			cin >> v >> W;
			A = C = 0;
			f(v);
			cout << A << ' ' << C << endl;
		}
	}

	return 0;
}