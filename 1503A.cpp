#include <iostream>
#include <vector>

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
		int z = 0;
		for(char c : s) if(c == '0') ++ z;
		if(s[0] == '0' || s.back() == '0' || (z&1)) {
			cout << "NO\n";
			continue;
		}
		int o = (n - z) / 2;
		string a(n, ' '), b(n, ' ');	
		for(int i = 0; i < n; ++i) {
			if(s[i] == '1') {
				if(o-- > 0) a[i] = b[i] = '(';
				else a[i] = b[i] = ')';
			} else {
				if((++z) & 1) {
					a[i] = '(';
					b[i] = ')';
				} else {
					a[i] = ')';
					b[i] = '(';
				}
			}
		}
		cout << "YES\n" << a << '\n' << b << '\n';
	}

	return 0;
}