#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
using vi = vector<int>;

int main() {
	int n; cin >> n;
	vi a(n);
	for(int &x : a) cin >> x;
	
	int sa = accumulate(a.begin(), a.end(), 0);
	if(sa%2 == 0) {
		sa /= 2;
		vector<int> dp(sa+1, -1);
		dp[0] = -2;
		for(int i = 0; i < n; ++i) {
			int x = a[i];
			for(int y = sa; y >= x; --y) if(dp[y] == -1 && dp[y-x] != -1)
				dp[y] = i;
		}
		cerr << sa << endl;
		if(dp[sa] != -1) {
			vector<bool> S(n, false);
			while(sa) {
				int i = dp[sa];
				sa -= a[i];
				S[i] = true;
			}
			cout << "Second" << endl;
			while(true) {
				int i, j = 0;
				cin >> i;
				if(i <= 0) return 0;
				--i;
				while(!a[j] || S[j] == S[i]) ++j;
				cout << j+1 << endl;
				int m = min(a[i], a[j]);
				a[i] -= m;
				a[j] -= m;
			}
			return 0;
		}
	}

	cout << "First" << endl;
	while(true) {
		int i = 0, j;
		while(!a[i]) ++i;
		cout << i+1 << endl;
		cin >> j;
		if(j <= 0) return 0;
		--j;
		int m = min(a[i], a[j]);
		a[i] -= m;
		a[j] -= m;
	}

	return 0;
}