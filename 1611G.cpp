#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n, m;
		cin >> n >> m;
		int d = (n+m)/2;
		vector<vector<int>> cs[2];
		for(int i : {0, 1}) cs[i].resize(d);
		for(int i = 0; i < n; ++i) {
			string r;
			cin >> r;
			for(int j = 0; j < m; ++j) if(r[j] == '1')
				cs[(i+j)&1][(i+j)>>1].push_back(i);
		}
		int ans = 0;
		for(auto &c : cs) {
			vector<int> v;
			for(int y = 0; y < d; ++y) {
				int i = v.size()-1;
				for(int x : c[y]) {
					int w = y-x;
					if(i == -1 || v[i] < w) {
						v.push_back(w);
						++ i;
					} else {
						while(i > 0 && v[i-1] >= w) --i;
						v[i] = w;
					}
				}
			}
			ans += v.size();
		}
		cout << ans << '\n';
	}

	return 0;
}