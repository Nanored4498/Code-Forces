#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		string s;
		cin >> n >> m >> s;
		vector<int> l(n), r(n);
		l[0] = s[0] == '1' ? 0 : m+1;
		for(int i = 1; i < n; ++i)
			l[i] = s[i] == '1' ? 0 : l[i-1]+1;
		r[n-1] = s[n-1] == '1' ? 0 : m+1;
		for(int i = n-2; i >= 0; --i)
			r[i] = s[i] == '1' ? 0 : r[i+1]+1;
		for(int i = 0; i < n; ++i)
			s[i] = min(l[i], r[i]) <= m && (l[i] == 0 || l[i] != r[i]) ? '1' : '0';
		cout << s << '\n';
	}

	return 0;
}