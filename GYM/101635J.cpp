#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> a(n), b(n);
	for(int &x : a) cin >> x;
	for(int &x : b) cin >> x;

	ll sum[3] {0, 0, 0};
	for(int i = 0; i < n; ++i) {
		sum[i%3] += a[i];
	}

	ll ans[3] {0, 0, 0};
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < 3; ++j) {
			int m = (j-i+1)%3;
			if(m < 0) m += 3;
			ans[j] += b[i] * sum[m];
		}
	}

	for(int i = 0; i < 3; ++i) cout << ans[i] << ' ';
	cout << '\n';

	return 0;
}