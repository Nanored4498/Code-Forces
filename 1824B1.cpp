#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

const int MOD = 1e9+7;

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
using mi = mint<MOD>;

template <typename T>
struct AU {
	vector<T> _fact, _ifact, _bernoulli;

	AU(): _fact(1, 1) {}

	T fact(int n) {
		if(n >= _fact.size()) {
			int i = _fact.size();
			_fact.resize(n+1);
			for(; i <= n; ++i) _fact[i] = i * _fact[i-1];
		}
		return _fact[n];
	}

	T ifact(int n) {
		if(n >= _ifact.size()) {
			int i0 = _ifact.size();
			_ifact.resize(n+1);
			for(int j = n; j >= i0; --j) _ifact[j] = inv(fact(j));
		}
		return _ifact[n];
	}

	T C(int n, int k) {
		if(k < 0 || k > n) return 0;
		return fact(n) * ifact(k) * ifact(n-k);
	}

	T Bernoulli(int n) {
		if(n >= _bernoulli.size()) {
			int i = _bernoulli.size();
			_bernoulli.resize(n+1);
			while(i <= n) {
				_bernoulli[i] = 1;
				for(int j = 0; j < i; ++j)
					_bernoulli[i] -= C(i, j) * _bernoulli[j] / (i - j + 1);
			}
		}
		return _bernoulli[n];
	}

	// 0^p + 1^p + ... + N^p
	T sumPow(T N, int p) {
		if(p == 0) return N+1;
		T ans = pow(N, p+1) / (p+1) + pow(N, p) / 2;
		for(int k = 2; k <= p; ++k)
			ans += Bernoulli(k) * fact(p) * ifact(k) * ifact(p-k+1) * pow(N, p-k+1);
		return ans;
	}
};
AU<mi> utils;

int n, k;
vector<vi> G;

pair<mi, int> f(int u, int u0=-1) {
	vi c;
	mi ans = 0;
	int m = 1;
	for(int v : G[u]) if(v != u0) {
		auto [av, mv] = f(v, u);
		ans += av;
		m += mv;
		c.push_back(mv);
	}
	if(m != n) c.push_back(n-m);
	for(int m : c) {
		for(int l = max(k/2+1, k+m-n); l <= min(m, k); ++l)
			ans += utils.C(m, l) * utils.C(n-m, k-l);
	}
	return make_pair(ans, m);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> k;
	if(n == 1) {
		cout << "1\n";
		return 0;
	}
	G.resize(n);
	for(int i = 1; i < n; ++i) {
		int u, v;
		cin >> u >> v;
		--u; --v;
		G[u].push_back(v);
		G[v].push_back(u);
	}

	cout << (mi(n) - f(0).first / utils.C(n, k)) << endl;

	return 0;
}