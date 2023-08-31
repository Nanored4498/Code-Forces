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
	int i2i[20];
	i2i[0] = 0;
	i2i[1] = 1;
	i2i[10] = 2;
	i2i[11] = 3;
	while(t--) {
		int n; cin >> n;
		int mm[4]; for(int i = 0; i < 4; ++i) mm[i] = 1e9;
		while(n--) {
			int m, s; cin >> m >> s;
			s = i2i[s];
			mm[s] = min(m, mm[s]);
		}
		int ans = min(mm[3], mm[1]+mm[2]);
		cout << (ans == 1e9 ? -1 : ans) << '\n';
	}

	return 0;
}