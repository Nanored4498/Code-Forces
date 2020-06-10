#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi a(n);
	for(int i = 0; i < n; ++i) {
		cin >> a[i];
	}
	const int none = -90000000;
	vi d(61, none);
	int res = 0;
	for(int i = 0; i < n; ++i) {
		d[a[i]+30] = max(0, d[a[i]+30]+a[i]);
		for(int m = 0; m < a[i]+30; ++m) d[a[i]+30] = max(d[a[i]+30], d[m]+m-30);
		res = max(res, d[a[i]+30]);
		for(int m = a[i]+31; m <= 60; ++m) {
			d[m] += a[i];
			res = max(d[m], res);
		}
	}
	cout << res << "\n";

	return 0;
}