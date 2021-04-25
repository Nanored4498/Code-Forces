#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;
typedef tuple<int, int, int> t3i;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int T;
	cin >> T;
	while(T--) {
		int n, m;
		cin >> n >> m;
		vector<vi> b(n, vi(m));
		vector<t3i> ts;
		ts.reserve(n*m);
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j) {
				cin >> b[i][j];
				ts.emplace_back(b[i][j], i, j);
			}
		sort(ts.begin(), ts.end());
		ts.resize(m);
		vi count(m, 0);
		for(const t3i &t : ts) ++ count[get<2>(t)];
		int to = 0;
		for(const t3i &t : ts) {
			int j = get<2>(t);
			if(count[j] <= 1) continue;
			int i = get<1>(t);
			while(count[to] > 0) ++ to;
			swap(b[i][j], b[i][to]);
			-- count[j];
			++ count[to];
		}
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < m; ++j)
				cout << b[i][j] << " \n"[j+1==m];
	}

	return 0;
}