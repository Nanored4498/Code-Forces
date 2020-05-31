#include <iostream>

using namespace std; 

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	string s;
	cin >> t;
	while(t --> 0) {
		cin >> s;
		int res = 0, n = s.size(), one=0;
		for(int i = 0; i < n; ++i) {
			one = max(one, i);
			int v = 0, j = one;
			while(j < n && j-i >= v) {
				v = (v<<1)+s[j++]-'0';
				if(v == 0) one = j;
				if(j-i == v) ++res;
			}
		}
		cout << res << "\n";
	}

	return 0;
}