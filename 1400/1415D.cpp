#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	if(n > 64) return cout << "1\n", 0;
	vi a(n);
	for(int i = 0; i < n; ++i) cin >> a[i];
	int ans = n;
	for(int i = 1; i < n; ++i) {
		int x = 0;
		for(int j = i-1; j >= 0; --j) {
			x ^= a[j];
			int y = a[i], k = i+1;
			while(x <= y && k < n) y ^= a[k++];
			if(x > y) ans = min(ans, k-j-2);
		}
	}
	cout << (ans==n ? -1 : ans) << "\n";

	return 0;
}