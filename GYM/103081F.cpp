#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<ll> vl;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll R, N, K;
	cin >> R >> N >> K;
	vl a(R+1), b(N+1, 0);
	vector<vl> c(N+1);

	for(int i = 0; i <= N; ++i) {
		c[i].resize(i+1);
		c[i][0] = c[i][i] = 1;
		for(int j = 1; j < i; ++j)
			c[i][j] = (c[i-1][j-1] + c[i-1][j]) % K;
	}

	b[0] = b[1] = 1LL;
	for(int i = 2; i <= N; ++i)
		for(int j = 1; j < i; ++j)
			b[i] = (b[i] + c[i-2][j-1] * ((b[j] * b[i-1-j]) % K)) % K;
	
	a[1] = b[N-R+1];
	for(int i = 2; i <= R; ++i) {
		a[i] = b[N-R+i];
		for(int j = 1; j < i; ++j)
			a[i] = (a[i] - c[i-1][j] * ((a[i-j] * b[j+1]) % K)) % K;
		a[i] = (a[i] + K) % K;
	}

	cout << a[R] << endl;

	return 0;
}