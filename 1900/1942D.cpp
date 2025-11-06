#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t = 0;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		vector<vi> a(n);
		for(int i = 0; i < n; ++i) {
			a[i].resize(n);
			for(int j = i; j < n; ++j)
				cin >> a[i][j];
		}
		vector<vi> d(n);
		d[0] = {0, a[0][0]};
		sort(d[0].rbegin(), d[0].rend());
		d[0].resize(min((int) d[0].size(), k));
		for(int i = 1; i < n; ++i) {
			priority_queue<int, vi, greater<int>> q(d[i-1].begin(),d[i-1].end());
			for(int j = 0; j <= i; ++j) {
				if(j >= 2) {
					for(int x : d[j-2]) {
						x += a[j][i];
						if(q.size() < k) {
							q.push(x);
						} else if(x > q.top()) {
							q.pop();
							q.push(x);
						} else break;
					}
				} else {
					if(q.size() < k) {
						q.push(a[j][i]);
					} else if(a[j][i] > q.top()) {
						q.pop();
						q.push(a[j][i]);
					}
				}
			}
			while(!q.empty()) {
				d[i].push_back(q.top());
				q.pop();
			}
			reverse(d[i].begin(), d[i].end());
		}
		for(int x : d[n-1]) cout << x << ' ';
		cout << '\n';
	}

	return 0;
}