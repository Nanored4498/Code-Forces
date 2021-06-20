#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef long long ll;

vector<vi> G;
vi c;

bool col(int i, int x) {
	c[i] = x;
	for(int j : G[i]) {
		if(c[j] == -1 && !col(j, x^1)) return false;
		if(c[j] == x) return false;
	}
	return true;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		vector<ll> v(n), t(n);
		c.assign(n, -1);
		G.resize(n);
		for(int i = 0; i < n; ++i) G[i].clear();
		for(ll &x : v) cin >> x;
		for(ll &x : t) cin >> x;
		for(int e = 0; e < m; ++e) {
			int i, j;
			cin >> i >> j;
			--i; --j;
			G[i].push_back(j);
			G[j].push_back(i);
		}
		if(col(0, 0)) {
			ll s[2] {0, 0};
			for(int i = 0; i < n; ++i)
				s[c[i]] += t[i] - v[i];
			if(s[0] == s[1]) cout << "YES\n";
			else cout << "NO\n"; 
		} else {
			ll s = 0;
			for(ll x : t) s += x;
			for(ll x : v) s -= x;
			if(abs(s)&1) cout << "NO\n";
			else cout << "YES\n"; 
		}
	}

	return 0;	
}