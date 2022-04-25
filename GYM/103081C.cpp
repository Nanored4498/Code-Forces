#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
const int MAX_N = 1000;

struct Point {
	double x, y;
};

int r[MAX_N+5], s[MAX_N+5];

int find(int i) {
	if(r[i] == i) return i;
	return r[i] = find(r[i]);
}

void merge(int i, int j) {
	i = find(i);
	j = find(j);
	if(r[i] < r[j]) swap(i, j);
	r[j] = i;
	s[i] += s[j];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << fixed << setprecision(7);

	double X, Y;
	cin >> X >> Y;
	int N;
	cin >> N;
	vector<Point> ps(N);
	for(int i = 0; i < N; ++i) cin >> ps[i].x >> ps[i].y;

	vector<tuple<double, int, int>> E;
	E.reserve((N+2)*(N+1)/2);
	for(int i = 0; i < N; ++i) {
		E.emplace_back(pow(min(ps[i].x, Y-ps[i].y), 2), i, N);
		E.emplace_back(pow(min(X-ps[i].x, ps[i].y), 2), i, N+1);
		for(int j = 0; j < i; ++j) {
			double dx = ps[j].x - ps[i].x;
			double dy = ps[j].y - ps[i].y;
			E.emplace_back((dx*dx + dy*dy) / 4., i, j);
		}
	}

	sort(E.begin(), E.end());
	for(int i = 0; i < N+2; ++i) {
		r[i] = i;
		s[i] = 1;
	}
	for(auto [d, i, j] : E) {
		merge(i, j);
		if(find(N) == find(N+1)) {
			cout << sqrt(d) << endl;
			return 0;
		}
	}

	return 0;
}