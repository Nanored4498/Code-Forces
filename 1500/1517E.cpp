#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

const ll MOD = 998244353;

int n;
vector<ll> a;

void f(ll sc, ll sp, int x, int y, ll &ans) {
	for(int i = x; i < y; ++i) {
		if((i-x)&1) sc += a[i];
		else sp += a[i];
	}
	int p = x;
	int c = y-1;
	if((y-x)&1) --c;
	while(c >= p-1) {
		while(c-2 >= p-1 && sc >= sp) {
			sc -= a[c];
			sp += a[c];
			c -= 2;
		}
		if(sc >= sp) break;
		ans = (ans + (c - p + 1)/2 + 1) % MOD;
		if(p+2 > y) break;
		sp -= a[p];
		sc += a[p];
		p += 2;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		cin >> n;
		a.resize(n);
		for(int i = 0; i < n; ++i) cin >> a[i];
		if(n == 1) {
			cout << "1\n";
			continue;
		}
		ll ans = 0;
		f(0, 0, 0, n, ans);
		f(a[0], 0, 1, n, ans);
		if(n >= 3) {
			f(a[1]+a[2], a[0], 3, n, ans);
			f(a[n-1], a[n-2]+a[n-3], 0, n-3, ans);
		}
		if(n >= 4) {
			f(a[1]+a[2]+a[3], a[0], 4, n, ans);
			f(a[0]+a[n-1], a[n-2]+a[n-3], 1, n-3, ans);
		}
		if(n >= 6) {
			f(a[1]+a[2]+a[n-1], a[0]+a[n-2]+a[n-3], 3, n-3, ans);
		}
		if(n >= 7) {
			f(a[1]+a[2]+a[3]+a[n-1], a[0]+a[n-2]+a[n-3], 4, n-3, ans);
		}
		ll sp = a[0], sc = 0;
		for(int i = 1; i < n; ++i) sc += a[i];
		for(int i = 1; i < n-2; ++i) {
			sc -= a[i];
			sp += a[i];
			if(sc < sp) ans = (ans + 1) % MOD;
		}

		cout << ans << '\n';
	}

	return 0;
}