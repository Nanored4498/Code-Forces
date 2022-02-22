#include <iostream>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<bool> seen(2*max(n, m));
	int ans = 0;
	vector<int> Q, Q2;
	seen[n] = true;
	Q.push_back(n);
	while(!Q.empty()) {
		Q2.clear();
		for(int x : Q) {
			if(x == m) {
				cout << ans << '\n';
				return 0;
			}
			for(int y : {2*x, x-1}) if(y >= 0 && y < seen.size() && !seen[y]) {
				seen[y] = true;
				Q2.push_back(y);
			}	
		}
		++ ans;
		swap(Q, Q2);
	}

	return 0;
}