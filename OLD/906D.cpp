#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
typedef long long ll;

int n, m;
vector<int> w;

int phi(int x) {
	static unordered_map<int, int> m;
	if(m.count(x)) return m[x];
	int x0 = x, ans = 1;
	for(int d = 2; d <= x/d; ++d) if(x%d == 0) {
		int e = 1;
		while(x%d == 0) {
			x /= d;
			e *= d;
		}
		ans *= (e - e/d);
	}
	if(x > 1) ans *= (x - 1);
	return m[x0] = ans;
}

pair<ll, bool> f(int l, int r, int mod=m) {
	if(l > r || w[l] == 1 || mod == 1) return {1, true};
	ll a = w[l], ans = 1, pm = phi(mod);
	auto [b, exact] = f(l+1, r, pm);
	if(!exact) b += pm;
	while(b) {
		if(b&1) {
			ans *= a;
			if(ans >= mod) {
				exact = false;
				ans %= mod;
			}
		}
		a *= a;
		if((b >>= 1) && (a >= mod)) {
			exact = false;
			a %= mod;
		}
	}
	return {ans, exact};
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	cin >> n >> m;
	w.resize(n);
	for(int &x : w) cin >> x;

	int q;
	cin >> q;
	while(q--) {
		int l, r;
		cin >> l >> r;
		cout << (f(l-1, r-1).first % m) << '\n';
	}

	return 0;
}