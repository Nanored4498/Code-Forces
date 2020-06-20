#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, x, ans=0;
	cin >> n;
	vi ia(n+1), d(n, 0);
	for(int i = 0; i < n; ++i) {
		cin >> x;
		ia[x] = i;
	}
	for(int i = 0; i < n; ++i) {
		cin >> x;
		int k = (n+ia[x]-i) % n;
		++ d[k];
		ans = max(ans, d[k]);
	}
	cout << ans << "\n";

	return 0;
}