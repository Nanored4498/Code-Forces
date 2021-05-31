#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		vector<int> u(n);
		vector<vector<ll>> s(n);
		for(int i = 0; i < n; ++i) cin >> u[i];
		for(int i = 0; i < n; ++i) {
			ll si;
			cin >> si;
			s[u[i]-1].push_back(si);
		}
		sort(s.rbegin(), s.rend(), [](const vector<ll> &a, const vector<ll> &b) {
			return a.size() < b.size();
		});
		for(int i = 0; i < n; ++i) {
			sort(s[i].rbegin(), s[i].rend());
			for(int j = 1; j < s[i].size(); ++j) s[i][j] += s[i][j-1];
		}
		for(int k = 1; k <= n; ++k) {
			ll score = 0;
			for(int i = 0; i < n && s[i].size() >= k; ++i) {
				score += s[i][s[i].size() - (s[i].size()%k) - 1];
			}
			cout << score << ' ';
		}
		cout << '\n';
	}

	return 0;
}