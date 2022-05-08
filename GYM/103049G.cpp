#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
typedef vector<int> vi;
typedef vector<double> vd;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, r, m;
	cin >> n >> r >> m;
	vi t(m+1), d(m);
	vd p(m);
	for(int i = 0; i < m; ++i) cin >> t[i] >> p[i] >> d[i];
	t[m] = n;
	int md = r-n;
	vector<vd> E(m+1, vd(md));
	E[m].assign(md, 0);
	double e0 = n, e1 = 50'000*n;
	for(int step = 0; step < 37; ++step) {
		double e = (e0+e1)/2.;
		E[0][0] = e;
		for(int i = m-1; i>=0; --i) {
			for(int a = md-1; a>=0; --a) {
				double bad = E[0][0];
				if(a+d[i] < md) bad = min(bad, t[i+1]-t[i]+d[i]+E[i+1][a+d[i]]);
				E[i][a] = p[i]*(t[i+1]-t[i]+E[i+1][a]) + (1.-p[i])*bad;
			}
		}
		E[0][0] += t[0];
		if(E[0][0] < e) e1 = e;
		else e0 = e;
	}
	cout << fixed << setprecision(7) << E[0][0] << endl;

	return 0;
}