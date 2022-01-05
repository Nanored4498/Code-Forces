#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
#define LSB(i) ((i)&(-(i)))

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int q;
	cin >> q;
	while(q--) {
		int n;
		string s, t;
		cin >> n >> s >> t;
		vector<int> p[26], count(n+1, 0);
		for(int i = 0; i < n; ++i) p[s[i]-'a'].push_back(i);
		for(auto &x : p) reverse(x.begin(), x.end());
		const auto cost = [&](int j) { int ans = j; for(int k = j+1; k > 0; k -= LSB(k)) ans -= count[k]; return ans; };
		ll ans = 1e12, tmp = 0;
		for(int i = 0; i < n; ++i) {
			int x = t[i]-'a';
			for(int c = 0; c < x; ++c)
				if(!p[c].empty()) ans = min(ans, tmp + cost(p[c].back()));
			if(p[x].empty()) break;
			tmp += cost(p[x].back());
			for(int k = p[x].back()+1; k <= n; k += LSB(k)) ++ count[k];
			p[x].pop_back();
		}
		cout << (ans == 1e12 ? -1 : ans) << '\n';
	}

	return 0;
}