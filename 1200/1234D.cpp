#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

struct SegmentTree {
	vector<int> v;
	int start;

	SegmentTree(int n): start(1) {
		while(start < n) start <<= 1;
		v.assign(start << 1, 0);
	}

	void set(int i, int x) {
		v[i+=start] = x;
		while((i >>= 1) > 0) v[i] = v[i<<1] | v[(i<<1)+1];
	}

	int querry(int a, int b) {
		int xa = 0, xb = 0;
		for(a += start, b += start; a <= b; a >>= 1, b >>= 1) {
			if(a&1) xa = xa | v[a++];
			if(!(b&1)) xb = v[b--] | xb;
		}
		return xa | xb;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string s;
	int q;
	cin >> s >> q;
	SegmentTree st(s.size());
	for(int i = 0; i < s.size(); ++i) st.set(i, 1<<(s[i]-'a'));
	while(q--) {
		int t, i;
		cin >> t >> i;
		if(t == 1) {
			char c; cin >> c;
			st.set(i-1, 1<<(c-'a'));
		} else {
			int j; cin >> j;
			int x = st.querry(i-1, j-1);
			int ans = 0;
			while(x) {
				if(x&1) ++ans;
				x >>= 1;
			}
			cout << ans << '\n';
		}
	}

	return 0;
}