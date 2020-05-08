#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cout.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vector<int> m(n), c(k);
	for(int i = 0; i < n; ++i) cin >> m[i];
	for(int i = 0; i < k; ++i) cin >> c[i];
	sort(m.begin(), m.end());

	int ans = 0;
	for(int i = 0; i < n; ++i)
		ans = std::max(ans, (n-i+c[m[i]-1]-1) / c[m[i]-1]);
	cout << ans << "\n";
	for(int i = 0; i < ans; ++i) {
		cout << (n-i+ans-1) / ans;
		for(int j = i; j < n; j += ans)
			cout << " " << m[j];
		cout << "\n";
	}

	return 0;
}