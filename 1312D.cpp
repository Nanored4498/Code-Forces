#include <iostream>

using namespace std;

#define MOD 998244353
#define MAX 200000
#define LL long long

LL modinv(LL a, LL m) { 
	LL m0 = m; 
	LL y = 0, x = 1;
	while (a > 1) {
		LL q = a / m; 
		LL t = m;
		m = a % m, a = t; 
		t = y;
		y = x - q * y; 
		x = t; 
	}
	if(x < 0) x += MOD;
	return x; 
}

LL fac[MAX], inv_fac[MAX];

LL invf(int i) {
	if(inv_fac[i] < 0) inv_fac[i] = modinv(fac[i], MOD);
	return inv_fac[i];
}

LL parmi(int k, int n) {
	return (((fac[n] * invf(k)) % MOD) * invf(n-k)) % MOD;
}
	
int main() {
	int n, m;
	cin >> n >> m;
	if(n < 3) {
		cout << 0 << endl;
		return 0;
	}

	fac[0] = 1;
	inv_fac[0] = 1;
	for(int i = 1; i < MAX; i++)
		fac[i] = (fac[i-1]*i) % MOD,
		inv_fac[i] = -1;

	LL A = 0, B = 0, C = n-2;
	for(int i = 1; i < n-1; i++) A = (A + parmi(i-1, n-3)) % MOD;
	for(int i = n-1; i <= m; i++) B = (B + parmi(n-2, i-1)) % MOD;
	LL res = (((A * B) % MOD) * C) % MOD;
	cout << res << endl;

	return 0;
}