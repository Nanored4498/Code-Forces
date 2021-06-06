#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
typedef long long ll;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	map<vi, vector<string>> m;
	vi cnt;
	for(int i = 0; i < n; ++i) {
		cnt.assign(26, 0);
		string s;
		cin >> s;
		for(char c : s) ++ cnt[c-'a'];
		m[cnt].push_back(s);
	}

	ll ans = 0;
	for(auto &[_, ss] : m) {
		ans += 1337LL * (n - ss.size()) * ss.size();
		ans += 2LL * ss.size() * (ss.size()-1);
		sort(ss.begin(), ss.end());
		int eq = 0;
		for(int i = 1; i < ss.size(); ++i) {
			if(ss[i] == ss[i-1]) ++ eq;
			else eq = 0;
			ans -= 4LL * eq;
		}
	}
	ans /= 2;
	cout << ans << endl;

	return 0;
}