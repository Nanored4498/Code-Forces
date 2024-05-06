#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

template <int MOD>
struct mint {
	typedef long long ll;
	static const int mod = MOD;
	int x;
	constexpr mint(): x(0) {}
	constexpr mint(ll v) {
		x = v % mod;
		if(x < 0) x += mod;
	}

	friend mint pow(mint a, ll p) {
		mint ans = 1;
		for(; p; p >>= 1, a *= a) if(p&1) ans *= a;
		return ans;
	}
	friend mint inv(mint a) { return pow(a, mod-2); }

	explicit operator int() const { return x; }
	friend bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
	friend bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
	friend bool operator<=(const mint &a, const mint &b) { return a.x <= b.x; }
	friend bool operator<(const mint &a, const mint &b) { return a.x < b.x; }
	friend ostream& operator<<(ostream &stream, const mint &v) { return stream << v.x; }
	friend istream& operator>>(istream &stream, mint &v) { return stream >> v.x; }
	mint& operator+=(const mint &b) { if((x += b.x) >= mod) x -= mod; return *this; }
	mint& operator-=(const mint &b) { if((x -= b.x) < 0) x += mod; return *this; }
	mint& operator*=(const mint &b) { x = (1LL * x * b.x) % mod; return *this; }
	mint& operator/=(const mint &b) { return (*this) *= inv(b); }
	mint& operator%=(const mint &b) { x %= b.x; return *this; }
	mint& operator++() { return (*this) += 1; }
	mint& operator--() { return (*this) -= 1; }
	mint operator-() { return -x; }
	friend mint operator+(mint a, const mint &b) { return a += b; }
	friend mint operator-(mint a, const mint &b) { return a -= b; }
	friend mint operator*(mint a, const mint &b) { return a *= b; }
	friend mint operator/(mint a, const mint &b) { return a /= b; }
};
const int MOD = 998244353;
using mi = mint<MOD>;
const int ML = 1e6+5;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	vector<mi> f(ML);
	f[0] = 1;
	for(int i = 1; i < ML; ++i) f[i] = f[i-1]*i;
	while(t--) {
		int l, n;
		cin >> l >> n;
		mi ans = f[l] / (f[2*n] * f[l-2*n]);
		const int mk = (l-2*n)/2;
		for(int k = 0; k <= mk; ++k) {
			ans -= (f[l-n-2*k] * f[n-1+k]) / (f[n] * f[l-2*n-2*k] * f[n-1] * f[k]);
		}
		ans *= 2;
		cout << ans << '\n';
	}
	return 0;
}