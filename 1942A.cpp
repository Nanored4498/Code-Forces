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

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		if(k == n) {
			for(int i = 0; i < n; ++i) cout << "1 ";
			cout << "\n";
		} else if(k == 1) {
			for(int i = 0; i < n; ++i) cout << i+1 << ' ';
			cout << "\n";
		} else {
			cout << "-1\n";
		}
	}

	return 0;
}