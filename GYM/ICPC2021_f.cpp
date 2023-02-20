#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <iomanip>

using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<vector<pll>> a(n);
	vector<array<ll, 6>> b(m);
	for(auto &p : a) {
		int na; cin >> na;
		p.resize(na);
		for(auto &[x, y] : p) cin >> x >> y;
	}
	for(auto &t : b) for(auto &x : t) cin >> x;

	double ans = 0.;
	for(const auto &p : a) {
		double mintan = -1.;
		for(const auto &[x0, y0, z0, x1, y1, z1] : b) {
			ll dxt = x1-x0, dyt = y1-y0;
			ll lt2 = dxt*dxt + dyt*dyt;
			bool ok = true;
			double maxtan = 0;
			for(const auto &[x, y] : p) {
				ll dx = x-x0, dy = y-y0;
				ll proj = dx*dxt + dy*dyt;
				if(proj < 0 || proj > lt2) {
					ok = false;
					break;
				}
				double pr = (double) proj / lt2;
				double xp = x0-x + pr * (x1-x0);
				double yp = y0-y + pr * (y1-y0);
				double zp = z0 + pr * (z1-z0);
				maxtan = max(maxtan, sqrt(xp*xp + yp*yp) / zp);
			}
			if(!ok) continue;
			mintan = mintan < 0. ? maxtan : min(mintan, maxtan);
		}
		// cerr << mintan << ' ' << atan(mintan)*180./M_PI << endl;
		if(mintan < 0.) return cout << "impossible\n", 0;
		ans = max(ans, mintan);
	}
	cout << fixed << setprecision(9) << atan(ans)*180./M_PI << endl;

	return 0;
}