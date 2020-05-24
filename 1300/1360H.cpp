#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, m;
	cin >> t;
	while(t --> 0) {
		cin >> n >> m;
		vector<ll> a(n);
		string s;
		for(int i = 0; i < n; ++i) {
			cin >> s;
			a[i] = 0;
			ll p = 1;
			for(int j = 0; j < m; ++j) {
				if(s[m-1-j] == '1') a[i] += p;
				p <<= 1;
			}
		}
		sort(a.begin(), a.end());
		ll res = ((1LL << m) - n - 1LL) >> 1;
		for(int i = 0; i < n; ++i) {
			if(a[i] <= res) ++ res;
			else break;
		}
		s = "\n";
		for(int i = 0; i < m; ++i) {
			if(res % 2 == 1) s = '1'+s;
			else s = '0'+s;
			res /= 2;
		}
		cout << s;
	}

	return 0;
}