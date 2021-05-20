#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
const int MAX_N = 1000;
const double EPS = 5e-6;

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
	
	double mi = 0., ma = sqrt(X*X + Y*Y);
	while(ma-mi > 2*EPS && (ma-mi)/(ma+mi) > EPS) {
		for(int i = 0; i < N+2; ++i) {
			r[i] = i;
			s[i] = 1;
		}

		double d = (ma+mi) / 2.;
		for(int i = 0; i < N; ++i) {
			if(ps[i].x <= d || ps[i].y >= Y-d) merge(i, N);
			if(ps[i].x >= X-d || ps[i].y <= d) merge(i, N+1);
		}
		if(find(N) == find(N+1)) {
			ma = d;
			continue;
		}

		double d2 = 4*d*d;
		for(int i = 0; i < N; ++i)
			for(int j = i+1; j < N; ++j) {
				if(find(i) == find(j)) continue;
				double dx = ps[j].x - ps[i].x;
				double dy = ps[j].y - ps[i].y;
				if(dx*dx + dy*dy <= d2) {
					merge(i, j);
					if(find(N) == find(N+1)) goto diag;
				}
			}
		
		nodiag:
		mi = d;
		continue;

		diag:
		ma = d;
	}	

	double d = (ma+mi) / 2.;
	cout << d << '\n';

	return 0;
}