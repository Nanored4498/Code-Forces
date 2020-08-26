#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

bool win(ll s, ll e) {
	if(e&1) return !(s&1);
	if(2*s > e) return s&1;
	if(4*s > e) return true;
	return win(s, e/4);
}

bool lose(ll s, ll e) {
	if(2*s > e) return true;
	return win(s, e/2);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	bool w=false, l=true;
	for(int i = 0; i < t; ++i) {
		ll s, e;
		cin >> s >> e;
		if(!(l^w)) continue;
		if(l) {
			w = win(s, e);
			l = lose(s, e);
		} else {
			w = !win(s, e);
			l = !lose(s, e);
		}
	}
	cout << w << " " << l << "\n";

	return 0;
}