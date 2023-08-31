#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

using namespace std;
using ll = long long;
using vi = vector<int>;

const int MOD = 1e9+7;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		vi a(64, 0), b;
		while(n--) {
			int x; cin >> x;
			b = a;
			b[x] += 1;
			for(int y = 0; y < 64; ++y) b[x&y] = (b[x&y] + a[y]) % MOD;
			swap(a, b);
		}
		int ans = 0;
		for(int x = 0; x < 64; ++x) {
			int l = 0;
			for(int i = 0; i < 6; ++i) if((x>>i)&1) ++l;
			if(l == k) ans = (ans + a[x]) % MOD;
		}
		cout << ans << '\n';
	}

	return 0;
}