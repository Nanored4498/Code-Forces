#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

using namespace std;
using ll = long long;
using vi = vector<int>;

int ind;
bool solve(const string &s, int l) {
	const ll MOD = 1e9+7;
	const ll p = 26;
	const int n = s.size();
	ll h = 0, pl = 1;
	for(int i = 0; i < l; ++i) {
		h = (p*h + s[i]) % MOD;
		pl = (pl * p) % MOD;
	}
	unordered_map<int, int> m;
	m[h] = 0;
	for(int i = 1; i <= n-l; ++i) {
		h = (p*h - s[i-1]*pl + s[i+l-1]) % MOD;
		if(h < 0) h += MOD;
		if(m.count(h)) m[h] = -1;
		else m[h] = i;
	}
	bool ans = false;
	ind = n;
	for(auto [h, i] : m) if(i != -1) {
		ans = true;
		ind = min(ind, i);
	}
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string s;
	cin >> s;
	int l = 1;
	for(int add = s.size()/2; add > 0; add /= 2) {
		while(l+add < s.size() && !solve(s, l + add))
			l += add;
	}
	solve(s, l+1);
	cout << s.substr(ind, l+1) << '\n';

	return 0;
}