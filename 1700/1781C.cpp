#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int t; cin >> t;
	while(t--) {
		int n;
		string s;
		cin >> n >> s;
		vector<pair<int, char>> a(26, {0,0});
		for(char c : s) ++a[c-'a'].first;
		for(int i = 0; i < 26; ++i) a[i].second = 'a'+i;
		sort(a.rbegin(), a.rend());
		int ans = n;
		string A;
		for(int i = 1; i <= 26; ++i) if((n%i) == 0) {
			int w = n/i, c = 0;
			for(int j = 0; j < 26; ++j) c += abs(a[j].first - (j < i ? w : 0));
			c /= 2;
			if(c < ans) {
				ans = c;
				A = s;
				auto b = a;
				for(char &c : A) {
					int j = 0; while(b[j].second != c) ++j;
					if(j < i && b[j].first <= w) continue;
					--b[j].first;
					j = 0; while(b[j].first >= w) ++j;
					++b[j].first; 
					c = b[j].second;
				}
			}
		}
		cout << ans << '\n' << A << '\n';
	}

	return 0;
}