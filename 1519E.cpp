#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<pii> p(n);
	map<pll, int> m;
	vector<set<int>> G;
	for(int i = 0; i < n; ++i) {
		ll a, b, c, d;
		cin >> a >> b >> c >> d;

		ll g = __gcd(a, b);
		pll x {a/g, b/g};
		if(!m.count(x)) {
			m[x] = G.size();
			G.emplace_back();
		}
		p[i].first = m[x];
		G[m[x]].insert(i);

		g = __gcd(c, d);
		x = {c/g, d/g};
		if(!m.count(x)) {
			m[x] = G.size();
			G.emplace_back();
		}
		p[i].second = m[x];
		G[m[x]].insert(i);
	}

	vector<pii> ans;
	

	return 0;
}