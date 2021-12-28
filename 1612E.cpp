#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

using namespace std;
const int MK = 20;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	unordered_map<int, array<int, MK+1>> m;
	while(n--) {
		int x, k;
		cin >> x >> k;
		++ m[x][k];
	}
	double ans = 0.;
	vector<int> c;
	for(int k = 1; k <= min((int) m.size(), MK); ++k) {
		vector<pair<double, int>> ms;
		ms.clear();
		for(const auto &[i, v] : m) {
			double s = 0;
			for(int j = 1; j <= MK; ++j)
				s += v[j] * min(1., double(j)/k);
			ms.emplace_back(s, i);
		}
		sort(ms.rbegin(), ms.rend());
		double a = 0.;
		for(int i = 0; i < k; ++i) a += ms[i].first;
		if(a > ans) {
			ans = a;
			c.clear();
			for(int i = 0; i < k; ++i) c.push_back(ms[i].second);
		}
	}
	cout << c.size() << endl;
	for(int x : c) cout << x << ' ';
	cout << endl;

	return 0;
}