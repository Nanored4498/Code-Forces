//https://codeforces.com/problemset/problem/1355/E
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll N, A, R, M, sh = 0;
	cin >> N >> A >> R >> M;
	M = min(M, A+R);
	vector<ll> h(N);
	for(int i = 0; i < N; ++i) {
		cin >> h[i];
		sh += h[i];
	}
	sort(h.begin(), h.end());
	ll a = 0, r = sh-N*h[0];
	sh /= N;
	ll res = R*r;
	for(int i = 1; i < N; ++i) {
		if(h[i-1] <= sh && sh <= h[i]) {
			ll a2 = a + i*(sh-h[i-1]);
			ll r2 = r - (N-i)*(sh-h[i-1]);
			if(a2 > r2) res = min(res, r2*M + (a2-r2)*A);
			else res = min(res, a2*M + (r2-a2)*R);
		}
		if(h[i-1] <= sh+1 && sh+1 <= h[i]) {
			ll a2 = a + i*(sh+1-h[i-1]);
			ll r2 = r - (N-i)*(sh+1-h[i-1]);
			if(a2 > r2) res = min(res, r2*M + (a2-r2)*A);
			else res = min(res, a2*M + (r2-a2)*R);
		}
		ll dh = h[i] - h[i-1];
		a += i * dh;
		r -= (N-i) * dh;
		if(a > r) res = min(res, r*M + (a-r)*A);
		else res = min(res, a*M + (r-a)*R);
	}
	cout << res << "\n";

	return 0;
}