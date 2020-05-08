#include <bits/stdc++.h>
using namespace std;

#define MAX_A 1000000000LL
#define ll long long
#define ld long double

int n;
ll k, a[100000], res[100000];

ll D(ll ai, ll b) { return ai + 3*b*(1-b); }

ll get_k(ll A) {
	ll r = 0;
	for(int i = 0; i < n; ++i) {
		if(A > a[i]) continue;
		ld delta = 9. + 12.*(ld)(a[i]-A);
		ll b = (ll) ((3. + sqrt(delta)) / 6.);
		while(D(a[i], b+1) >= A) ++b;
		while(D(a[i], b) < A) --b;
		b = min(b, a[i]);
		r += b;
	}
	return r;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> k;
	for(int i = 0; i < n; ++i) cin >> a[i];
	ll A0 = -3*MAX_A*(3+MAX_A), A1 = MAX_A;
	while(A0 < A1) {
		ll m = (A0+A1) / 2;
		if(A0+A1 < 0) --m;
		if(get_k(m) > k) A0 = m+1;
		else A1 = m;
	}
	ll k2 = get_k(A0);
	priority_queue<pair<ll, int>> last;
	for(int i = 0; i < n; ++i) {
		if(A0 > a[i]) { cout << 0; };
		ld delta = 9. + 12.*(ld)(a[i]-A0);
		ll b = (ll) ((3. + sqrt(delta)) / 6.);
		while(D(a[i], b+1) >= A0) ++b;
		while(D(a[i], b) < A0) --b;
		res[i] = min(b, a[i]);
		if(res[i] < a[i]) last.emplace(D(a[i], b), i);
	}
	for(int add = 0; add < k-k2; ++add) {
		auto p = last.top(); last.pop();
		int i = p.second;
		++ res[i];
		if(res[i] < a[i]) last.emplace(D(a[i], res[i]), i);
	}
	cout << res[0];
	for(int i = 1; i < n; ++i) cout << " " << res[i];

	return 0;
}