#include <iostream> 
#include <vector>
#include <algorithm>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, p, x;
	cin >> n;
	vector<pii> cs(n);
	for(int i = 0; i < n; ++i) cin >> cs[i].first >> cs[i].second;
	vector<vi> xs(n);
	int j = 0;
	cin >> p;
	for(int i = 0; i < p; ++i) {
		cin >> x;
		while(j < n && x > cs[j].second) ++j;
		if(j >= n) break;
		if(x >= cs[j].first) {
			xs[j].push_back(x);
			if(xs[j].size() > 2) return cout << "impossible\n", 0;
			if(j+1 < n && cs[j+1].first <= x) xs[j+1].push_back(x);
		}
	}

	vi ans;
	for(int i = 0; i < n; ++i) {
		while(xs[i].size() < 2) {
			int x = cs[i].second;
			if(i+1 < n && xs[i+1].size() >= 2) --x;
			bool ok = false;
			while(!ok) {
				ok = true;
				for(int y : xs[i]) if(y == x) {
					-- x;
					ok = false;
				}
			}
			ans.push_back(x);
			xs[i].push_back(x);
			if(i+1 < n && cs[i+1].first == x) xs[i+1].push_back(x);
		}
	}
	cout << ans.size() << "\n";
	for(int i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i+1 == ans.size()];

	return 0;
}