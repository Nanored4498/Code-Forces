#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

using namespace std;
using ai = array<int, 3>;
const int INF = -1e8;

struct SegmentTree {
	vector<pair<int, int>> v;
	int start;
	SegmentTree(int n): start(1) {
		while(start < n) start <<= 1;
		v.assign(start << 1, {INF, -1});
	}
	void set(int i, int x) {
		v[i+=start] = {x, i};
		while((i >>= 1) > 0) v[i] = max(v[i<<1], v[(i<<1)+1]);
	}
	auto querry(int a, int b) {
		pair<int, int> xa = {INF,-1}, xb = {INF,-1};
		for(a += start, b += start; a <= b; a >>= 1, b >>= 1) {
			if(a&1) xa = max(xa, v[a++]);
			if(!(b&1)) xb = max(v[b--], xb);
		}
		return max(xa, xb);
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	vector<ai> a(n);
	int I = 0;
	for(auto &[mi, su, ind] : a) {
		string s; cin >> s;
		su = 0;
		mi = 0;
		ind = ++I;
		for(char c : s)
			if(c == '(') ++ su;
			else mi = min(mi, --su);
	}
	sort(a.begin(), a.end());
	SegmentTree ms(n);
	for(int i = 0; i < n; ++i) ms.set(i, a[i][1]);

	vector<int> sol;
	int su = 0;
	for(int i = 0; i < n; ++i) {
		auto it = lower_bound(a.begin(), a.end(), ai{-su, INF, 0});
		int ind0 = it - a.begin();
		auto [m, mi] = ms.querry(ind0, n-1);
		su += m;
		if(m == INF || su < 0) {
			cout << "impossible\n";
			return 0;
		}
		ms.set(mi, INF);
		sol.push_back(a[mi][2]);
	}
	if(su == 0) for(int i : sol) cout << i << '\n';
	else cout << "impossible\n";

	return 0;
}