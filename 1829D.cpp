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

	int t; cin >> t;
	while(t--) {
		int n, m; cin >> n >> m;
		int n3 = 0, n2 = 0;
		int m3 = 0, m2 = 0;
		while(n%2 == 0) {
			n /= 2;
			++ n2;
		}
		while(n%3 == 0) {
			n /= 3;
			++ n3;
		}
		while(m%2 == 0) {
			m /= 2;
			++ m2;
		}
		while(m%3 == 0) {
			m /= 3;
			++ m3;
		}
		if(n == m && n3 >= m3 && n2 <= m2 && (m2-n2) <= (n3-m3)) cout << "YES\n";
		else cout << "NO\n";
	}

	return 0;
}