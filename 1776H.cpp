#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n; cin >> n;
		vector<int> a(n), b(n);
		for(int &x : a) cin >> x;
		for(int &x : b) cin >> x;
		int i = n-1, j = n-1;
		while(j >= 0) {
			if(b[i] == a[j]) --i;
			-- j;
		}
		cout << i+1 << '\n';
	}

	return 0;
}