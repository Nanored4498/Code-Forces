#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		string s;
		cin >> n >> s;
		int x = 1, y = 1;
		for(char c : s) if(c=='R') ++x; else ++y;
		int i = 0;
		while(i < (int)s.size() && s[i]==s[0]) ++i;
		if(s[0]=='D') swap(x, y);
		ll ans = i+1 + (n-x);
		if(y > 1) {
			ans += ll(n-i)*(n-1);
			ans -= (x-i)*(y-1);
			ans += x + y - i - 2;
		}
		cout << ans << '\n';
	}

	return 0;
}