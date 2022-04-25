#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>

using namespace std;
typedef pair<int,int> pii;
typedef complex<double> cpx;

void FFT(vector<cpx> &P, bool invert=false) {
	int n = P.size();
	constexpr double pi = 3.141592653589793238462643383279502884L;
	cpx g(cos(2*pi/n), sin(2*pi/n));
	if(invert) g = 1./g;

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
		cpx wl = g;
		for(int i = len; i < n; i <<= 1) wl *= wl;
		for(int i = 0; i < n; i += len) {
			cpx w = 1;
			for(int j = 0; j < half; ++j) {
				cpx even = P[i+j];
				cpx odd = P[i+half+j] * w;
				P[i+j] = even + odd;
				P[i+half+j] = even - odd;
				w *= wl;
			}
		}
	}

	// Divide by n if inverse
	if(invert) {
		cpx inv_n = 1./n;
		for(cpx &x : P) x *= inv_n;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n; cin >> n;
	pii a, b;
	cin >> a.first >> a.second;
	cin >> b.first >> b.second;
	vector<pii> M(n);
	for(pii &m : M) cin >> m.first >> m.second;
	sort(M.begin(), M.end());
	for(const pii &m : M) {
		pii c {a.first+m.first, a.second+m.second};
		if(c == b) {
			cout << "Alice wins\n";
			return 0;
		}
		if(c.first < 1 || c.first > n || c.second < 1 || c.second > n) continue;
		pii d {b.first-c.first, b.second-c.second};
		auto it = lower_bound(M.begin(), M.end(), d);
		if(it == M.end()) continue;
		if(*it == d) {
			cout << "Alice wins\n";
			return 0;
		}
	}
	if(n < 4) {
		bool s[4][4];
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j <= n; ++j)
				s[i][j] = false;
		s[b.first][b.second] = true;
		for(const pii &m : M) {
			pii c {b.first+m.first, b.second+m.second};
			if(c.first < 1 || c.first > n || c.second < 1 || c.second > n) continue;
			s[c.first][c.second] = true;
			for(const pii &m2 : M) {
				pii d {c.first+m2.first, c.second+m2.second};
				if(d.first < 1 || d.first > n || d.second < 1 || d.second > n) continue;
				s[d.first][d.second] = true;
			}
		}
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j <= n; ++j)
				if(!s[i][j]) {
					cout << "tie " << i << ' ' << j << endl;
					return 0;
				}
		cout << "Bob wins" << endl;
		return 0;
	}

	vector<cpx> P(2*n+1, 0.);
	P[n] = .5;
	for(const pii &m : M) P[m.first+n] += 1.;
	int size = 1;
	while(size < 2*P.size()) size <<= 1;
	P.resize(size, 0);
	FFT(P);
	for(int i = 0; i < size; ++i) P[i] *= P[i];
	FFT(P, true);
	P[2*n] += .75;
	for(int i = 1; i <= n; ++i) {
		int diff = i - b.first;
		long long count = round(P[diff+2*n].real());
		if(count >= n) continue;
		vector<bool> s(n+1, false);
		if(i == b.first) s[b.second] = true;
		for(const pii &m : M) {
			pii c {b.first+m.first, b.second+m.second};
			if(c.first < 1 || c.first > n || c.second < 1 || c.second > n) continue;
			if(c.first == i) s[c.second] = true;
			auto it = lower_bound(M.begin(), M.end(), pii{i-c.first, -n});
			for(; it != M.end() && c.first+it->first == i; ++it) {
				int j = c.second+it->second;
				if(1 <= j && j <= n) s[j] = true;
			}
		}
		for(int j = 1; j <= n; ++j) if(!s[j]) {
			cout << "tie " << i << ' ' << j << endl;
			return 0;
		}
		return 1;
	}

	return 0;
}