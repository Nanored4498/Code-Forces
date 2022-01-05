#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<int> r, s, ct;

int find(int i) {
	if(r[i] == i) return i;
	return r[i] = find(r[i]);
}

void merge(int i, int j) {
	i = find(i); j = find(j);
	if(i == j) return;
	if(s[i] < s[j]) swap(i, j);
	r[j] = i;
	s[i] += s[j];
	ct[i] = min(ct[i], ct[j]);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		r.resize(n);
		s.assign(n, 1);
		ct.resize(n);
		unordered_map<int, vector<pair<int, int>>> row, col;
		for(int i = 0; i < n; ++i) {
			r[i] = i;
			int x, y;
			cin >> x >> y >> ct[i];
			row[x].emplace_back(y, i);
			col[y].emplace_back(x, i);
		}
		for(auto vs : {&row, &col}) for(auto &[_, v] : *vs) {
			sort(v.begin(), v.end());
			for(int i = 1; i < v.size(); ++i) if(v[i].first <= v[i-1].first+k)
				merge(v[i-1].second, v[i].second);
		}
		int ans = -1;
		vector<int> ts;
		for(int i = 0; i < n; ++i) if(find(i) == i) ts.push_back(ct[i]);
		sort(ts.begin(), ts.end());
		while(!ts.empty() && ans < ts.back()) {
			++ ans;
			ts.pop_back();
		}
		cout << ans << '\n';
	}

	return 0;
}