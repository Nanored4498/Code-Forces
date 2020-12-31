#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string s;
		cin >> s;
		vector<bool> changed(s.size(), false);
		int ans = 0;
		for(int i = 1; i < s.size(); ++i) {
			if(s[i] == s[i-1] && !changed[i-1]) {
				changed[i] = true;
				++ ans;
			} else if(i > 1 && s[i] == s[i-2] && !changed[i-2]) {
				changed[i] = true;
				++ ans;
			}
		}
		cout << ans << "\n";
	}

	return 0;
}