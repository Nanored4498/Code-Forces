#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string s;
	cin >> s;
	if(s[0] != s.back()) {
		cout << s[0] << s.back() << '\n';
	} else {
		char c0 = s[0];
		for(char c : s) if(c != c0) {
			cout << c0 << c << c0 << '\n';
			return 0;
		}
		cout << c0 << '\n';
	}

	return 0;
}