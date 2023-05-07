#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t; cin >> t;
	string s, r = "codeforces";
	while(t--) {
		cin >> s;
		int ans = 0;
		for(int i = 0; i < (int)r.size(); ++i) if(s[i] != r[i]) ++ ans;
		cout << ans << '\n';
	}

	return 0;
}