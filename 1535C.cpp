#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string s;
		cin >> s;
		int n = s.size();
		ll ans = 0;
		int first = 0, last = 0;
		while(last < n && s[last] == '?')
			ans += ++last;
		for(int i = last; i < n; ++i) {
			if(s[i] == '?') {
				
			} else {
				if(s[i] == '0' && s[last] == '0' && ((i-last)&1)) first = last+1;
				else if(s[i] == '0' && s[last] == '1' && !((i-last)&1)) first = last+1;
				else if(s[i] == '1' && s[last] == '0' && !((i-last)&1)) first = last+1;
				else if(s[i] == '1' && s[last] == '1' && ((i-last)&1)) first = last+1;
				last = i;
			}
			ans += i - first + 1;
		}
		cout << ans << '\n';
	}

	return 0;
}