#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = __int128_t;
using pll = pair<ll, ll>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<pll> p(n);
	for(auto &[x, y] : p) cin >> x >> y;
	ll tot_area = 0;
	pll b {0, 0};
	for(int i = 2; i < n; ++i) {
		ll ax = p[i-1].first - p[0].first;		
		ll ay = p[i-1].second - p[0].second;	
		ll bx = p[i].first - p[0].first;		
		ll by = p[i].second - p[0].second;		
		ll det = ax*by - ay*bx;
		tot_area += det;
		b.first += det * (p[0].first + p[i-1].first + p[i].first);
		b.second += det * (p[0].second + p[i-1].second + p[i].second);
	}
	tot_area *= 3;
	ll g = gcd(gcd(b.first, b.second), tot_area);
	b.first /= g;
	b.second /= g;
	tot_area /= g;
	cerr << b.first << ' ' << b.second << ' ' << tot_area << endl;

	if(tot_area == 1) { // remove barycenter from polygon
		for(int i = 0; i < n; ++i) if(p[i] == b) {
			p[i] = p.back();
			p.pop_back();
		}
	}

	const auto cmp = [&](pll a, pll b) {
		
	};

	return 0;
}