#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, P;
	cin >> N;
	double res = 0, ar, x, x2, y, y2;
	while(N --> 0) {
		cin >> P;
		double x0, y0;
		cin >> x0 >> y0;
		x = x0;
		y = y0;
		ar = 0.;
		while(--P > 0) {
			cin >> x2 >> y2;
			ar += (x2-x) * (y+y2) / 2.;
			x = x2;
			y = y2;
		}
		res += std::abs(ar + (x0-x) * (y+y0) / 2.);
	}
	cout << (long long) res << "\n";

	return 0;
}