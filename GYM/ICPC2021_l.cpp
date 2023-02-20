#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int w, h;
	cin >> h >> w;
	vector<string> m(w);
	for(string &s : m) cin >> s;
	int di = 0, dj = 0, r = 1, dd = 1, dir = 3;
	int dirx[] {0, 1, 0, -1};
	int diry[] {1, 0, -1, 0};
	int n = 0, sn = 0;
	vector<vector<pair<int, int>>> tree(2), nt;
	for(int i = 0; i < w; ++i) for(int j = 0; j < h; ++j) {
		if(m[i][j] == '.') tree[1].emplace_back(i, j);
		else tree[0].emplace_back(i, j);
	}
	if(tree[1].empty()) tree.pop_back();
	while(true) {
		bool allone = true;
		for(auto &v : tree) if(v.size() != 1) {
			allone = false;
			break;
		}
		if(allone) {
			sn += tree.size() * n;
			cout << fixed << setprecision(9) << double(sn)/double(w*h) << '\n';
			cout << n << '\n';
			vector<pair<int, int>> a;
			for(auto &v : tree) a.emplace_back(w-v[0].first, v[0].second+1);
			sort(a.begin(), a.end());
			for(auto x : a) cout << '(' << x.second << ',' << x.first << ')' << " \n"[x==a.back()];
			return 0;
		}
		di += dirx[dir];
		dj += diry[dir];
		if(--dd == 0) {
			if((dir = (dir+1)%4)&1) ++r;
			dd = r;
		}
		for(auto &v : tree) {
			if(v.size() == 1) {
				sn += n;
				continue;
			}
			nt.emplace_back();
			auto &b = nt.emplace_back();
			auto &a = nt[nt.size()-2];
			for(auto &[i, j] : v) {
				const int i2 = i + di;
				const int j2 = j + dj;
				if(i2 < 0 || i2 >= w || j2 < 0 || j2 >= h || m[i2][j2] == '.') a.emplace_back(i, j);
				else b.emplace_back(i, j);
			}
			if(a.empty()) {
				a = move(b);
				nt.pop_back();
			} else if(b.empty()) nt.pop_back();
		}
		swap(tree, nt);
		nt.clear();
		++n;
	}

	return 0;
}