#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <iomanip>

using namespace std;
using ll = long long;
using vi = vector<int>;

struct UF {
	vector<int> r;
	UF(int n): r(n, -1) {}
	int find(int i) {
		if(r[i] < 0) return i;
		return r[i] = find(r[i]);
	}
	void merge(int i, int j) {
		i = find(i); j = find(j);
		if(i == j) return;
		if(r[i] > r[j]) swap(i, j);
		r[i] += r[j];
		r[j] = i;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	double X, Y;
	cin >> X >> Y;
	int n;
	cin >> n;
	vector<pair<double, double>> ps(n);
	for(auto &[x, y] : ps) cin >> x >> y;
	UF uf(n+2);
	vector<tuple<double, int, int>> events;
	for(int i = 0; i < n; ++i) {
		auto [x, y] = ps[i];
		for(int j = 0; j < i; ++j) {
			double dx = x - ps[j].first;
			double dy = y - ps[j].second;
			double d = sqrt(dx*dx + dy*dy) / 2.;
			events.emplace_back(d, i, j);
		}
		events.emplace_back(min(x, Y-y), i, n);
		events.emplace_back(min(y, X-x), i, n+1);
	}
	sort(events.begin(), events.end());
	for(auto [d, i, j] : events) {
		uf.merge(i, j);
		if(uf.find(n) == uf.find(n+1)) {
			cout << fixed << setprecision(5) << d << '\n';
			return 0;
		}
	}

	return 0;
}