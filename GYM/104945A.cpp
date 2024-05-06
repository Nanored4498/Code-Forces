#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N; cin >> N;

	int ind[128];
	const char swerc[] = "SWERC";
	int val[5];
	for(int i = 0; i < 5; ++i) {
		ind[swerc[i]] = i;
		val[i] = i*(N+1);
	}

	vector<pair<int, int>> cards(N);
	for(auto &[a, b] : cards) {
		char c;
		cin >> c >> b;
		a = ind[c];
	}

	int ans = 0;
	vector<int> v(N);
	do {
		for(int i = 0; i < N; ++i)
			v[i] = val[cards[i].first] + cards[i].second;
		vector<int> s;
		for(int x : v) {
			auto it = upper_bound(s.begin(), s.end(), x);
			if(it == s.end()) s.push_back(x);
			else *it = x;
		}
		ans = max(ans, (int) s.size());
	} while(next_permutation(val, val+4));
	cout << N - ans << '\n';

	return 0;
}