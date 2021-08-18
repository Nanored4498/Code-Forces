#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		string s;
		cin >> n >> s;
		int i = 0;
		while(i < n && s[i] == '?') ++i;
		if(i == n) {
			for(int j = 0; j < n; ++j) s[j] = (j&1) ? 'B' : 'R';
		} else {
			for(int j = i; j > 0; --j)
				s[j-1] = (s[j] == 'R') ? 'B' : 'R';
			for(int j = i+1; j < n; ++j) if(s[j] == '?')
				s[j] = (s[j-1] == 'R') ? 'B' : 'R';
		}
		cout << s << '\n';
	}

	return 0;
}