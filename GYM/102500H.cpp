#include <iostream> 
#include <vector>
#include <iomanip>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> pii;
typedef vector<ll> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vi dh(n+1);
	ll h0, h1;
	cin >> h0;
	for(int i = 1; i <= n; ++i) {
		cin >> h1;
		dh[i] = h1-h0;
		h0 = h1;
	}
	cout << setprecision(8);

	while(k --> 0) {
		double gd;
		cin >> gd;
		ll g = gd*10+.5;
		vector<pii> pred;
		pred.emplace_back(0, 0);
		ll s = 0;
		ld ans = -1;
		for(int i = 1; i <= n; ++i) {
			s += dh[i] - g;
			if(s < pred.back().second) pred.emplace_back(i, s);
			else {
				int j = pred.size()-1;
				for(int a = pred.size()/2; a > 0; a >>= 1)
					while(j-a >= 0 && pred[j-a].second <= s)
						j -= a;
				ld a = i - pred[j].first;
				ll slope = -1e10;
				if(i < n) slope = max(slope, dh[i+1]);
				if(i-a > 0) slope = max(slope, dh[i-a]);
				if(slope > -2e9) {
					slope -= g;
					a += min(ld(1), max(ld(0), ld(s - pred[j].second) / ld(-slope)));
				}
				ans = max(ans, a);
			}
		}
		cout << ans << "\n";
	}

	return 0;
}