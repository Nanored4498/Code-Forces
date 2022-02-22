#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<int> a(n);
	for(int &x : a) cin >> x;
	sort(a.begin(), a.end());
	for(int i = 2; i < n; ++i) {
		if(a[i-2]+a[i-1] > a[i]) {
			cout << "YES\n";
			return 0;
		}
	}
	cout << "NO\n";
	return 0;
}