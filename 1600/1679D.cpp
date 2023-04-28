#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
using vb = vector<bool>;

int n, m;
ll k;
vector<vi> G;
vi a;
vl r;
vb seen;

int B;
ll g(int i) {
	if(a[i] > B) return 0;
	if(!seen[i]) {
		if(r[i] != -1) {
			seen[i] = true;
			return r[i] = k;
		}
		r[i] = 1;
		for(int j : G[i]) r[i] = max(r[i], 1+g(j));
		seen[i] = true;
	}
	return r[i];
}

bool f(int b) {
	B = b;
	r.assign(a.size(), -1);
	seen.assign(a.size(), false);
	ll x = 0;
	for(int i = 0; i < n; ++i) x = max(x, g(i));
	return x >= k;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m >> k;
	a.resize(n);
	for(int &x : a) cin >> x;
	G.resize(n);
	while(m--) {
		int u, v;
		cin >> u >> v;
		G[u-1].push_back(v-1);
	}
	vi b = a;
	sort(b.rbegin(), b.rend());
	if(!f(b[0])) {
		cout << "-1\n";
		return 0;
	}
	int i = 0, add = n/2;
	while(add) {
		int j = i+add;
		if(j < n && f(b[j])) i = j;
		else add /= 2;
	}
	cout << b[i] << '\n';

	return 0;
}