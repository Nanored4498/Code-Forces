#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

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

// Need to compute a genrator g of T before
template <typename T>
void NTT(vector<T> &P, T g, bool invert=false) {
	int n = P.size();
	assert((T::mod-1) % n == 0);
	g = pow(g, (T::mod-1) / n);
	if(invert) g = inv(g);

	// swap indices with their mirror binary
	for(int i = 1, j = 0; i < n; ++i) {
		int bit = n >> 1;
		for(; j&bit; bit >>= 1) j ^= bit;
		j ^= bit;
		if(i < j) swap(P[i], P[j]);
	}

	// Transform
	for(int len = 2; len <= n; len <<= 1) {
		int half = len >> 1;
		T wl = g;
		for(int i = len; i < n; i <<= 1) wl *= wl;
		for(int i = 0; i < n; i += len) {
			T w = 1;
			for(int j = 0; j < half; ++j) {
				T even = P[i+j];
				T odd = P[i+half+j] * w;
				P[i+j] = even + odd;
				P[i+half+j] = even - odd;
				w *= wl;
			}
		}
	}

	// Divide by n if inverse
	if(invert) {
		T inv_n = inv(T(n));
		for(T &x : P) x *= inv_n;
	}
}

template <typename T>
void NTT_mult_inplace(vector<T> &P, vector<T> &Q, T g) {
	int size = 1, n = P.size() + Q.size() - 1;
	while(size < n) size <<= 1;
	P.resize(size, 0);
	Q.resize(size, 0);
	NTT<T>(P, g);
	NTT<T>(Q, g);
	for(int i = 0; i < size; ++i) P[i] *= Q[i];
	NTT<T>(P, g, true);
	P.resize(n);
}

const int MOD = 998244353;
typedef mint<MOD> mi;
const mi g = 3;
AU<mi> utils;

mi f(int n, int k) {
	k = min(n, k);
	vector<mi> P(k+1), Q(k+1);
	for(int i = 0; i <= k; ++i) {
		P[i] = i&1 ? -utils.ifact(i) : utils.ifact(i);
		Q[i] = pow(mi(i), n) * utils.ifact(i);
	}
	NTT_mult_inplace(P, Q, g);
	mi ans = 0;
	for(int i = 0; i <= k; ++i) ans += P[i];
	return ans-1;
}

int main() {
	int n, k;
	cin >> n >> k;
	if(n == 1 || k == 1) {
		cout << "1\n";
		return 0;
	}

	vector<bool> p(n+1, true);
	vector<int> m(n+1, 1);
	for(int i = 2; i <= n; ++i) if(p[i]) {
		m[i] = -1;
		for(int j = i+i; j <= n; j += i) {
			if((j/i)%i == 0) m[j] = 0;
			else m[j] = -m[j];
			p[j] = false;
		}
	}

	mi ans=0, tmp=0;
	int last=n+1;
	for(int i = 1; i <= n; ++i) if(m[i] != 0) {
		int d = (n+i-1)/i;
		ans += m[i] * (d==last ? tmp : tmp=f(d, k));
		last = d;
	}
	cout << ans << endl;

	return 0;
}