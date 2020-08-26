#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		string s;
		cin >> n >> k >> s;
		vi co(26, 0);
		for(char c : s) ++ co[c-'a'];
		for(int l = n; l >= 1; --l) {
			int o = l / __gcd(l, k);
			int u = 0;
			for(int c = 0; c < 26; ++c) u += co[c]/o;
			if(u*o >= l) {
				cout << l << "\n";
				break;
			}
		} 
	}

	return 0;
}