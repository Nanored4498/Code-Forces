#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string s;
		cin >> s;
		if(s[0] != s.back()) s[0] = s.back();
		cout << s << '\n';
	}

	return 0;
}