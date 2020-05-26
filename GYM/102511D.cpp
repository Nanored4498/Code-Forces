#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;
typedef vector<int> vi;
typedef vector<bool> vb;

#define MAXI 1000001

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi is(n), ns(MAXI, 0), ne(MAXI, 0);
	vb open(n), seen(MAXI, false);
	int curr = 0;
	for(int i = 0; i < n; ++i) {
		char c;
		cin >> c >> is[i];
		open[i] = c == 's';
		if(!seen[is[i]]) {
			++ curr;
			seen[is[i]] = true;
		}
		if(ns[is[i]] == 0 && ne[is[i]] == 0) --curr;
		if(open[i]) ++ns[is[i]];
		else if(ns[is[i]] > 0) --ns[is[i]];
		else ++ ne[is[i]];
		if(ns[is[i]] == 0 && ne[is[i]] == 0) ++curr;
	}
	int m = curr, p = 0;
	for(int p2 = 1; p2 < n; ++p2) {
		int i = is[p2-1];
		if(ns[i] == ne[i]) {
			if(ns[i] == 0) --curr;
			if(open[p2-1]) ++ns[i];
			else --ns[i];
			ne[i] = ns[i];
			if(ns[i] == 0) ++curr;
		}
		if(curr > m) {
			m = curr;
			p = p2;
		}
	}
	cout << p+1 << " " << m << "\n";

	return 0;
}