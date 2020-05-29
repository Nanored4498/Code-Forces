#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
typedef long long ll;
typedef vector<ll> vl;
#define M_PI 3.14159265358979323846

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;
	vl X(N), Y(N), G(N);
	for(int i = 0; i < N; ++i) {
		cin >> X[i] >> Y[i] >> G[i];
	}
	if(N == 2) {
		cout << "0\n";
		return 0;
	}

	ll res = 1e18;
	for(int i = 0; i < N; ++i) {
		vector<pair<double, ll>> th;
		for(int j = 0; j < N; ++j) if(j != i) {
			double t = atan2(Y[j]-Y[i], X[j]-X[i]);
			th.emplace_back(t, G[j]);
			if(t > 0) t -= M_PI;
			else t += M_PI;
			th.emplace_back(t, -G[j]);
		}
		sort(th.begin(), th.end());
		int j = 0;
		while(th[j].second < 0) {
			th.push_back(th[j]);
			++ j;
		}
		double t0 = th[j].first;
		ll D = 0;
		for(int k = j+1; k < th.size(); ++k) {
			if(th[k].second < 0) continue;
			if(th[k].first-t0 < M_PI) D += th[k].second;
			else D -= th[k].second;
		}
		res = min(res, abs(D));
		for(++j; j < th.size(); ++j) {
			D -= th[j-1].second;
			D -= th[j].second;
			if(th[j].second > 0) res = min(res, abs(D));
		}
	}
	cout << res << endl;

	return 0;
}