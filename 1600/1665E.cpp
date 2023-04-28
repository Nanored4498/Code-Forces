#include <iostream>
#include <array>
#include <vector>

using namespace std;
typedef array<int, 31> A;

template<typename T, typename OP>
struct SegmentTree {
	OP op;
	const T e;
	vector<T> v;
	int start;

	SegmentTree(int n, const OP &op, const T &e): op(op), e(e), start(1) {
		while(start < n) start <<= 1;
		v.assign(start << 1, e);
	}

	void set(int i, const T &x) {
		v[i+=start] = x;
		while((i >>= 1) > 0) v[i] = op(v[i<<1], v[(i<<1)+1]);
	}
	void add(int i, const T &x) { set(i, v[start+i]+x); }

	T querry(int a, int b) {
		T xa = e, xb = e;
		for(a += start, b += start; a <= b; a >>= 1, b >>= 1) {
			if(a&1) xa = op(xa, v[a++]);
			if(!(b&1)) xb = op(v[b--], xb);
		}
		return op(xa, xb);
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	const int inf = 2e9;
	A e;
	for(int i = 0; i < 31; ++i) e[i] = inf;

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		SegmentTree st(n, [](const A &a, const A &b) {
			A c;
			int i = 0, j = 0;
			for(int k = 0; k < 31; ++k)
				if(a[i] < b[j]) c[k] = a[i++];
				else c[k] = b[j++];
			return c;
		}, e);
		for(int i = 0; i < n; ++i) {
			A a = e; cin >> a[0];
			st.set(i, a);
		}
		int q; cin >> q;
		while(q--) {
			int l, r;
			cin >> l >> r;
			A a = st.querry(l-1, r-1);
			int ans = inf;
			for(int i = 1; i < 31; ++i) if(a[i] != inf)
				for(int j = 0; j < i; ++j)
					ans = min(ans, a[i]|a[j]);
			cout << ans << '\n';
		}
	}

	return 0;
}