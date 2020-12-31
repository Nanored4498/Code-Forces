#include <iostream>
#include <vector>
#include <queue>

using namespace std;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef tuple<int, int, int> t3i;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, s=0;
		cin >> n;
		vi a(n+1);
		for(int i = 1; i <= n; ++i) {
			cin >> a[i];
			s += a[i];
		}
		if(s % n == 0) {
			int m = s / n;
			vector<t3i> ans;
			for(int i = 2; i <= n; ++i) {
				int x = a[i] / i;
				a[i] -= x*i;
				if(x == 0) continue;
				a[1] += x*i;
				ans.emplace_back(i, 1, x);
			}

		} else cout << "-1\n";
	}

	return 0;
}