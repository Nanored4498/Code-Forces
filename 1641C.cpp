#include <iostream>
#include <set>
#include <vector>

using namespace std;
#define LSB(i) ((i)&(-(i)))

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	cin >> n >> q;
	set<int> nz;
	for(int i = 1; i <= n; ++i) nz.emplace_hint(nz.end(), i);
	vector<int> maxL(n+1, 0);
	while(q--) {
		int t; cin >> t;
		if(t) {
			int j; cin >> j;
			auto it = nz.find(j);
			if(it == nz.end()) cout << "NO\n";
			else {
				int l, r, ml = 0;
				if(it == nz.begin()) l = 1;
				else { l = *(--it)+1; ++it; }
				if(++it == nz.end()) r = n;
				else r = *it-1;
				for(; r; r -= LSB(r)) ml = max(ml, maxL[r]);
				cout << (ml >= l ? "YES" : "N/A") << '\n';
			} 
		} else {
			int l, r, x;
			cin >> l >> r >> x;
			if(x) {
				for(; r <= n; r += LSB(r)) maxL[r] = max(maxL[r], l);
			} else {
				auto it = nz.lower_bound(l);
				while(it != nz.end() && *it <= r) it = nz.erase(it);
			}
		}
	}

	return 0;
}