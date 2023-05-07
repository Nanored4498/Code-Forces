#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

int main() {
	int n;
	cin >> n;
	vector<pll> ps(n);
	for(auto &[x, y] : ps) cin >> x >> y;
	ll a[2] = {0, 0};
	vector<bool> rm(n, false);
	for(int step = 0; step < n-2; ++step) {
		int ind = 0;
		ll ma = 1e15;
		for(int i = 0; i < n; ++i) if(!rm[i]) {
			int j = (i+n-1) % n;
			while(rm[j]) j = (j+n-1) % n;
			int k = (i+1) % n;
			while(rm[k]) k = (k+1) % n;
			ll dx1 = ps[i].first - ps[j].first;
			ll dy1 = ps[i].second - ps[j].second;
			ll dx2 = ps[k].first - ps[j].first;
			ll dy2 = ps[k].second - ps[j].second;
			ll a = abs(dx1*dy2 - dy1*dx2);
			if(a < ma) {
				ma = a;
				ind = i;
			}
		}
		cerr << "  " << step << ' ' << ma << ' ' << ind << endl;
		rm[ind] = true;
		a[step&1] += ma;
	}
	cerr << a[0] << ' ' << a[1] << endl;
	return 0;
}