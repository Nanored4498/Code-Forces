#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<tuple<int, int, string>> t(n);
	int ind = n;
	for(auto &[s, o, c] : t) {
		o = --ind;
		cin >> c >> s;
		s *= 10;
		int mi = 10, ma = 0, x;
		for(int i = 0; i < 6; ++i) {
			cin >> x;
			mi = min(mi, x);
			ma = max(ma, x);
			s += x;
		}
		s -= mi+ma;
	}
	sort(t.rbegin(), t.rend());
	int l = 100, count = 0;
	for(auto &[s, o, c] : t) {
		if(count >= 3 && s != l) break;
		++ count;
		l = s;
		cout << c << ' ' << s << '\n';
	}

	return 0;
}