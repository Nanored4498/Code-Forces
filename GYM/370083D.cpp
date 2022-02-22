#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, m;
vector<int> a;
vector<vector<int>> G;

int f(int i, int p=-1, int c=0) {
	if(a[i]) ++ c;
	else c = 0;
	if(c > m) return 0;
	if(p != -1 && G[i].size() == 1) return 1;
	int ans = 0;
	for(int j : G[i]) if(j != p) ans += f(j, i, c);
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	a.resize(n);
	for(int &x : a) cin >> x;
	G.resize(n);
	while(--n) {
		int x, y;
		cin >> x >> y;
		-- x; -- y;
		G[x].push_back(y);
		G[y].push_back(x);
	}
	cout << f(0) << endl;

	return 0;
}