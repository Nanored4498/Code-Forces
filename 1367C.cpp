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
		n += k+1;
		for(int i = 0; i < k; ++i) s.push_back('0');
		s.push_back('1');
		int l = -k-1;
		int ans = 0;
		for(int i = 0; i < n; ++i) if(s[i] == '1') {
			int d = i-l-1-k;
			ans += d / (k+1);
			l = i;
		}
		cout << ans << "\n";
	}

	return 0;
}