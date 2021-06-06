#include <iostream>
#include <vector>

#include "../CPHeaders/lazy_segment_tree.h"

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vi a(n);
	for(int &x : a) cin >> x;
	vi last(n+1, -1), lastId(n), dp(n);

	for(int i = 0; i < n; ++i) {
		int &l = last[a[i]];
		lastId[i] = l;
		dp[i] = i > 0 ? dp[i-1] : 0;
		if(l != -1) dp[i] += i-l;
		l = i;
	}

	LazySegmentTree<int> st(n+1, [](int x, int y)->int { return min(x, y); }, [](int x, int _)->int { return x; }, numeric_limits<int>::max());
	for(int j = 1; j < k; ++j) {
		for(int i = 0; i < n; ++i) st.v[st.start+i] = dp[i];
		st.init();
		for(int i = j; i < n; ++i) {
			if(lastId[i] > 0) st.add(0, lastId[i]-1, i-lastId[i]);
			dp[i] = st.querryAll();
		}
	}
	cout << dp[n-1] << endl;

	return 0;
}