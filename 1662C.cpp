#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<ll> vi;
typedef vector<vi> vvi;
const ll MOD = 998'244'353;


vvi mult(const vvi &a, const vvi &b) {
	int n = a.size(), m = b.size(), p = b[0].size();
	vvi c(n, vi(p, 0));
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < p; ++j)
			for(int k = 0; k < m; ++k)
				c[i][j] = (c[i][j]+a[i][k]*b[k][j]) % MOD;
	return c;
}

vvi pow(const vvi &a, int p) {
	int n = a.size();
	vvi b(n, vi(n, 0)), c=a;
	for(int i = 0; i < n; ++i) b[i][i] = 1;
	while(p) {
		if(p&1) b = mult(b, c);
		p >>= 1;
		c = mult(c, c);
	}
	return b;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, k;
	cin >> n >> m >> k;
	vvi A(n, vi(n, 0));
	while(m--) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		A[a][b] = A[b][a] = 1;
	}
	vvi A2 = mult(A, A);
	vvi x(2*n, vi(n)), M(2*n, vi(2*n, 0));
	for(int i = 0; i < n; ++i) {
		ll d = 0;
		for(int j = 0; j < n; ++j) {
			x[i][j] = A2[i][j];
			x[n+i][j] = A[i][j];
			M[i][j] = A[i][j];
		}
		x[i][i] = 0;
		M[n+i][i] = 1;
		M[i][n+i] = (1-A2[i][i]+MOD)%MOD;
	}
	vvi Mk = pow(M, k-1);
	vvi y = mult(Mk, x);
	ll ans = 0;
	for(int i = 0; i < n; ++i) ans += y[n+i][i];
	ans %= MOD;
	cout << ans << endl;

	return 0;
}