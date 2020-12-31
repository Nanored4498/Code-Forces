#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n;
	cin >> t;
	while(t--) {
		cin >> n;
		vi a(n);
		for(int i = 0; i < n; ++i) cin >> a[i];
		int j = n-1;
		while(j > 0 && a[j-1] >= a[j]) --j;
		while(j > 0 && a[j-1] <= a[j]) --j;
		cout << j << "\n";
	}

	return 0;
}