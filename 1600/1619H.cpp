#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	cin >> n >> q;
	vector<int> p(n), r(n), s(n);
	int m = sqrt(n);
	for(int &x : p) { cin >> x; --x; }
	for(int i = 0; i < n; ++i) {
		r[p[i]] = i;
		s[i] = i;
		for(int j = 0; j < m; ++j) s[i] = p[s[i]];
	}

	while(q--) {
		int t, i, j;
		cin >> t >> i >> j;
		-- i;
		if(t == 1) {
			-- j;
			r[p[i]] = j;
			r[p[j]] = i;
			swap(p[i], p[j]);
			for(int k : {i, j}) {
				s[k] = k;
				for(int l = 0; l < m; ++l) s[k] = p[s[k]];
				for(int l = 1; l < m; ++l) {
					k = r[k];
					s[k] = r[s[p[k]]];
				}
			}
		} else {
			int a = j/m, b = j%m;
			while(a--) i = s[i];
			while(b--) i = p[i];
			cout << i+1 << '\n';
		}
	}

	return 0;
}