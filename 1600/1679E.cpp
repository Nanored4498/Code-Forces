#include <iostream>
#include <vector>
#include <tuple>
#include <bit>

using namespace std;
using uint = unsigned int;

template <int MOD>
struct mint {
	typedef long long ll;
	static const int mod = MOD;
	int x;
	mint(): x(0) {}
	mint(ll v) {
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
using mi = mint<998244353>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	string s;
	cin >> n >> s >> q;

	// start / mask / count
	vector<tuple<int, uint, int>> p;
	vector<pair<int, int>> ps;
	int X = 0;
	for(int i = 0; i < n; ++i) {
		if(s[i] == '?') ++ X;
		p.emplace_back(i, 0, 0);
		p.emplace_back(i+1, 0, 0);
		for(int j = 0; j < (int)p.size(); ++j) {
			auto &[k, m, c] = p[j];
			--k;
			if(k >= 0 && (s[k]=='?' || s[i]=='?' || s[k]==s[i])) {
				if(s[k] != s[i]) {
					++c;
					if(s[k] == '?') m |= 1 << (s[i]-'a');
					else m |= 1 << (s[k]-'a');
				} else if(s[k] == '?' && k != i) ++c;
				ps.emplace_back(m, c);
			} else {
				p[j--] = p.back();
				p.pop_back();
			}
		}
	}

	vector<vector<mi>> ans(17, vector<mi>(1<<17, 0));
	for(int i = 0; i < 17; ++i) {
		vector<mi> pc(X+1);
		for(int c = 0; c <= X; ++c) pc[c] = pow(mi(i+1), X-c);
		for(auto [m, c] : ps) ans[i][m] += pc[c];
		for(int j = 0; j < 17; ++j) for(uint m = 0; m < (1<<17); ++m)
			if(m & (1<<j)) ans[i][m] += ans[i][m ^ (1<<j)];
	}

	while(q--) {
		string t;
		cin >> t;
		uint m = 0;
		for(char c : t) m |= 1 << (c-'a');
		cout << ans[popcount(m)-1][m] << '\n';
	}

	return 0;
}