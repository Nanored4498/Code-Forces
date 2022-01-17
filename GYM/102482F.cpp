#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	vector<int> s(n);
	int l = 0;
	for(int &x : s) {
		string w; cin >> w;
		x = w.size();
		l = max(l, x);
	}
	int ans = 0, lb = l;
	while(true) {
		int al = 1e6, i = 0;
		vector<pair<int, int>> r0, r1;
		while(i < n) {
			int p = s[i++], j = 0;
			for(; i < n && p+1+s[i] <= l; ++i) {
				while(j < r0.size() && r0[j].first+1 < p) ++j;
				int a = 0;
				for(int k : {j, j+1}) if(k < r0.size() && r0[k].first <= p+1)
					a = max(a, r0[k].second);
				++ a;
				if(a > ans) {
					ans = a;
					lb = l;
				}
				r1.emplace_back(p, a);
				p += 1+s[i];
			}
			if(i < n) al = min(al, p+1+s[i]-l);
			r0.clear();
			swap(r0, r1);
		}
		if(al == 1e6) break;
		l += al;
	}
	cout << lb << ' ' << ans << endl;

	return 0;
}