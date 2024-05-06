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
		int n;
		cin >> n;
		vector<bool> used(n+1, false);
		int m = 0;
		for(int i = 0; i < n; ++i) {
			while(used[m]) ++m;
			int a;
			cin >> a;
			int p = m-a;
			if(p < 0 || p >= n || used[p]) p = m;
			cout << p << ' ';
			used[p] = true;
		}
		cout << '\n';
	}

	return 0;
}