#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip> 

using namespace std;
typedef pair<int, int> pii;
typedef vector<double> vd;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vd x(n), y(n), dx(n), dy(n), s(n);
	double res = -1;
	for(int i = 0; i < n; ++i) {
		cin >> x[i] >> y[i] >> dx[i] >> dy[i] >> s[i];
		double no = sqrt(dx[i]*dx[i] + dy[i]*dy[i]);
		dx[i] /= no;
		dy[i] /= no;
		double proj_i = -dy[i]*x[i] + dx[i]*y[i];
		for(int j = 0; j < i; ++j) {
			double dot_v = -dy[i]*dx[j] + dx[i]*dy[j];
			double proj_j = -dy[i]*x[j] + dx[i]*y[j];
			double mt = 1e9;
			if(abs(dot_v) < 1e-9) {
				if(abs(proj_j-proj_i) > 1e-9) continue;
				double ti = (x[j]-x[i])*dx[i] + (y[j]-y[i])*dy[i];
				double tj = (x[i]-x[j])*dx[j] + (y[i]-y[j])*dy[j];
				if(ti < 0) {
					if(tj < 0) continue;
					mt = tj / s[j];
				} else {
					if(tj < 0) mt = ti / s[i];
					else mt = ti / (s[i] + s[j]);
				}
			} else {
				double tj = (proj_i - proj_j) / dot_v;
				if(tj < 0 && tj > -1e-8) tj = 0;
				if(tj < 0) continue;
				double x0 = x[j] + tj*dx[j];
				double y0 = y[j] + tj*dy[j];
				double ti = (x0-x[i])*dx[i] + (y0-y[i])*dy[i];
				if(ti < 0 && ti > -1e-8) ti = 0;
				if(ti < 0) continue;
				mt = max(ti/s[i], tj/s[j]);
			}
			if(res < 0) res = mt;
			else res = min(res, mt);
		}
	}
	if(res < 0) cout << "No show :(\n";
	else cout << fixed << setprecision(15) << res << "\n";

	return 0;
}