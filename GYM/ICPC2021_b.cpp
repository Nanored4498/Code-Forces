#include <iostream>
#include <vector>
#include <tuple>

using namespace std;
using ll = long long;

const int MAXN = 2005;
vector<pair<int, ll>> G[MAXN];
int s, k, t;

tuple<ll, ll, int> f(int u, int p) {
	ll a = 0, b = 0, bk = 0;
	int kt = 0;
	for(const auto &[v, w] : G[u]) if(v != p) {
		auto [av, bv, ktv] = f(v, u);
		if(av < 0) return {-1, 0, 0};
		a += av+2*w;
		if(ktv&1) bk = bv+w;
		else b = max(b, bv+w);
		kt |= ktv;
	}
	if(kt == 3) kt = 0;
	else if(kt == 1) {
		if(u == t) return {-1, 0, 0};
		b = max(b, bk);
	} else if(u == t) kt |= 2;
	else if(u == k && kt != 2) kt |= 1;
	// cerr << p << " -> " << u << ": " << a << ' ' << b << ' ' << kt << endl;
	return {a, b, kt}; 
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	cin >> n >> q;
	while(--n) {
		int u, v;
		ll w;
		cin >> u >> v >> w;
		G[u].emplace_back(v, w);
		G[v].emplace_back(u, w);
	}
	while(q--) {
		cin >> s >> k >> t;
		auto [a, b, kt] = f(s, 0);
		if(a < 0LL) cout << "impossible\n";
		else cout << a-b << '\n';
	}

	return 0;
}