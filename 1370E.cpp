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

	int n;
	cin >> n;
	string s, t;
	cin >> s >> t;
	int no = 0;
	for(char c : s) if(c == '1') ++no;
	for(char c : t) if(c == '1') --no;
	if(no != 0) return cout << "-1\n", 0;

	string r = "";
	for(int i = 0; i < n; ++i) if(s[i] != t[i]) r.push_back(s[i]);
	if(r.empty()) return cout << "0\n", 0;
	n = r.size();
	int i0 = 0;
	while(r[i0] == r[0]) ++i0;
	vi cs = {1}, cs2;
	for(int i = i0+1; i != i0; i = (i+1)%n) {
		if(r[i] != r[(i+n-1)%n]) cs.push_back(0);
		++ cs.back();
	}
	i0 = 0;
	int ans = 0;
	while(true) {
		++ ans;
		cs2.clear();
		int l=-1;
		for(int i = 0; i < cs.size(); ++i) {
			-- cs[i];
			if(cs[i] > 0) {
				if((i&1)==l) cs2.back() += cs[i];
				else cs2.push_back(cs[i]);
				l = i&1;
			}
		}
		if(cs2.empty()) break;
		if(cs2.size()&1) {
			cs2[0] += cs2.back();
			cs2.pop_back();
		}
		swap(cs, cs2);
	}
	cout << ans << "\n";

	return 0;
}