#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#define MOD 998244353

int main() {
	iostream::sync_with_stdio(false);

	int n;
	cin >> n;
	vector<pair<int, int>> s;
	int a, b;
	for(int i = 0; i < n; i++) {
		cin >> a >> b;
		s.emplace_back(a, b);
	}
	long long res = 1, bad1 = 1, bad2 = 1, inter = 1;
	sort(s.begin(), s.end());
	long long na = 1, nb = 1;
	for(int i = 1; i < n; i++) {
		if(s[i].first == s[i-1].first) {
			na += 1;
			bad1 = (bad1 * na) % MOD;
			if(s[i].second == s[i-1].second) {
				nb += 1;
				inter = (inter * nb) % MOD;
			} else nb = 1;
		} else {
			na = 1, nb = 1;
			if(s[i].second < s[i-1].second) inter = 0;
		}
	}
	for(int i = 0; i < n; i++) s[i] = {s[i].second, s[i].first};
	sort(s.begin(), s.end());
	nb = 1;
	for(int i = 1; i < n; i++) {
		res = (res * (i+1)) % MOD;
		if(s[i].first == s[i-1].first) {
			nb += 1;
			bad2 = (bad2 * nb) % MOD;
		} else nb = 1;
	}
	res = (res - bad1 - bad2 + inter + 2*MOD) % MOD;
	cout << res << "\n";
	return 0;
}