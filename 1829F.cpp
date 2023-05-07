#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		vi d(n);
		while(m--) {
			int u, v; cin >> u >> v;
			--u; --v;
			++d[u];
			++d[v];
		}
		map<int,int> v;
		for(int x : d) ++ v[x]; 
		auto it = v.begin();
		if(v.size() == 2) {
			int x = (++it)->first;
			cout << x << ' ' << x-1 << '\n';
		} else {
			int x = (++it)->first;
			int y = (++it)->first;
			if(v[y] == 1) swap(x, y);
			cout << x << ' ' << y-1 << '\n';
		}
	}

	return 0;
}