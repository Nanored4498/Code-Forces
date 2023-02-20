#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	while(t--) {
		int n, m;
		string s, t;
		cin >> n >> m >> s >> t;
		s.insert(s.end(), t.rbegin(), t.rend());
		n += m;
		int count = 0;
		for(int i = 1; i < n; ++i) if(s[i] == s[i-1]) ++ count;
		cout << (count <= 1 ? "YES" : "NO") << '\n';
	}

	return 0;
}