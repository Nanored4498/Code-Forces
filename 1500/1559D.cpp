#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

struct UF {
	vi uf, suf;

	UF(int n) {
		uf.resize(n);
		for(int i = 0; i < n; ++i) uf[i] = i;
		suf.assign(n, 1);
	}

	int find(int i) {
		if(uf[i] == i) return i;
		return uf[i] = find(uf[i]);
	}

	void merge(int i, int j) {
		i = find(i);
		j = find(j);
		if(suf[i] < suf[j]) swap(i, j);
		uf[j] = i;
		suf[i] += suf[j];
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m1, m2;
	cin >> n >> m1 >> m2;
	cout << n-1 - max(m1, m2) << '\n';
	bool s = m1 < m2;
	UF a(n+1), b(n+1);
	while(m1--) {
		int u, v;
		cin >> u >> v;
		a.merge(u, v);
	}
	while(m2--) {
		int u, v;
		cin >> u >> v;
		b.merge(u, v);
	}
	if(s) swap(a, b);
	int u = 1;
	for(int v = 1; v <= n; ++v)
		if(a.find(u) != a.find(v) && b.find(u) != b.find(v)) {
			a.merge(u, v);
			b.merge(u, v);
			cout << u << ' ' << v << '\n';
		}
	for(int v = 1; v <= n; ++v) if(a.find(v) != a.find(1)) {
		while(b.find(1) == b.find(u)) ++u;
		a.merge(u, v);
		b.merge(u, v);
		cout << u << ' ' << v << '\n';
	}
	return 0;
}