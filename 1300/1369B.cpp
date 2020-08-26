#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		string s, r;
		cin >> n >> s;
		int i = n-1;
		while(i >= 0 && s[i] == '1') --i;
		if(i == -1) cout << s << "\n";
		else {
			++i;
			int j = 0;
			while(j < n && s[j] == '0') r += s[j++];
			if(j < i) r += '0';
			while(i < n) r += s[i++];
			cout << r << "\n";
		}
	}

	return 0;
}