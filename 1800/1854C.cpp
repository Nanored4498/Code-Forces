#include <iostream>
#include <vector>

using namespace std;

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

	friend constexpr mint pow(mint a, ll p) {
		mint ans = 1;
		for(; p; p >>= 1, a *= a) if(p&1) ans *= a;
		return ans;
	}
	friend constexpr mint inv(mint a) { return pow(a, mod-2); }

	explicit operator int() const { return x; }
	friend bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
	friend bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
	friend bool operator<=(const mint &a, const mint &b) { return a.x <= b.x; }
	friend bool operator<(const mint &a, const mint &b) { return a.x < b.x; }
	friend ostream& operator<<(ostream &stream, const mint &v) { return stream << v.x; }
	friend istream& operator>>(istream &stream, mint &v) { return stream >> v.x; }
	constexpr mint& operator+=(const mint &b) { if((x += b.x) >= mod) x -= mod; return *this; }
	constexpr mint& operator-=(const mint &b) { if((x -= b.x) < 0) x += mod; return *this; }
	constexpr mint& operator*=(const mint &b) { x = (1LL * x * b.x) % mod; return *this; }
	constexpr mint& operator/=(const mint &b) { return (*this) *= inv(b); }
	constexpr mint& operator%=(const mint &b) { x %= b.x; return *this; }
	constexpr mint& operator++() { return (*this) += 1; }
	constexpr mint& operator--() { return (*this) -= 1; }
	constexpr mint operator-() { return -x; }
	friend constexpr mint operator+(mint a, const mint &b) { return a += b; }
	friend constexpr mint operator-(mint a, const mint &b) { return a -= b; }
	friend constexpr mint operator*(mint a, const mint &b) { return a *= b; }
	friend constexpr mint operator/(mint a, const mint &b) { return a /= b; }
};
constexpr int MOD = 1e9+7;
using mi = mint<MOD>;
using vi = vector<mi>;


int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<int> S(n);
	for(int &x : S) {
		cin >> x;
		-- x;
	}

	vector<vi> dp(m-1, vi(m, 0));
	constexpr mi i2 = mi(1)/mi(2);
	for(int s = 2*m-3; s > 0; --s) {
		const int my = min(s, m-1);
		for(int y = s/2+1; y <= my; ++y) {
			const int x = s-y;
			dp[x][y] = i2;
			if(y+1 >= m) dp[x][y] += i2 * (m-x);
			else dp[x][y] += i2 * dp[x][y+1];
			if(x+1 < y) dp[x][y] += i2 * dp[x+1][y];
		}
	}

	mi ans = m - S.back();
	for(int i = 1; i < n; ++i) ans += dp[S[i-1]][S[i]];
	cout << ans << '\n';

	return 0;
}