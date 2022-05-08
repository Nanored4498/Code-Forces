#include <iostream>
#include <array>
#include <cmath>
#include <iomanip>

using namespace std;
typedef pair<double, double> pd;
typedef array<pd, 3> A;
const int STEP = 30;

double dist(const pd &a, const pd &b) {
	const double x = a.first-b.first;
	const double y = a.second-b.second;
	return sqrt(x*x+y*y);
}

double f(const A &ps) {
	double x0 = 1e9, x1 = -1e9;
	double y0 = 1e9, y1 = -1e9;
	for(const pd &p : ps) {
		x0 = min(x0, p.first);
		x1 = max(x1, p.first);
		y0 = min(y0, p.second);
		y1 = max(y1, p.second);
	}
	double ans = 1e9;
	for(int step = STEP; step--;) {
		const auto xsearch = [&](double y) {
			double a = x0, b = x1, ans=1e9;
			for(int step2 = STEP; step2--;) {
				double a2 = a+(b-a)/3.;
				double b2 = a+2.*(b-a)/3.;
				double va=0., vb=0.;
				for(const pd &p : ps) {
					va += dist({a2, y}, p);
					vb += dist({b2, y}, p);
				}
				ans = min({ans, va, vb});
				if(va > vb) a = a2;
				else b = b2;
			}
			return ans;
		};
		double c = y0+(y1-y0)/3.;
		double d = y0+2.*(y1-y0)/3.;
		double vc = xsearch(c);
		double vd = xsearch(d);
		ans = min({ans, vc, vd});
		if(vc > vd) y0 = c;
		else y1 = d;
	}
	return ans;
}

int main() {
	A ps;
	for(pd &p : ps) cin >> p.first >> p.second;

	// BBox
	double x0 = 1e9, x1 = -1e9;
	double y0 = 1e9, y1 = -1e9;
	for(const pd &p : ps) {
		x0 = min(x0, p.first);
		x1 = max(x1, p.first);
		y0 = min(y0, p.second);
		y1 = max(y1, p.second);
	}

	double ans = 1e9;
	for(int step = STEP; step--;) {
		const auto xsearch = [&](double y) {
			double a = x0, b = x1, ans=1e9;
			for(int step2 = STEP; step2--;) {
				double a2 = a+(b-a)/3.;
				double b2 = a+2.*(b-a)/3.;
				double va=0., vb=0.;
				for(int i = 0; i < 3; ++i) {
					A qs = ps;
					qs[i] = {a2, y}; va = max(va, f(qs));
					qs[i] = {b2, y}; vb = max(vb, f(qs));
				}
				ans = min({ans, va, vb});
				if(va > vb) a = a2;
				else b = b2;
			}
			return ans;
		};
		double c = y0+(y1-y0)/3.;
		double d = y0+2.*(y1-y0)/3.;
		double vc = xsearch(c);
		double vd = xsearch(d);
		ans = min({ans, vc, vd});
		if(vc > vd) y0 = c;
		else y1 = d;
	}
	cout << fixed << setprecision(10) << ans << endl;
}