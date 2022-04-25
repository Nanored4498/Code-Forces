#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	cin >> n >> q;
	const int N = 100;
	const int M = N*N+N+1;
	ll a[M+1], b[N];
	for(int i = 1; i <= n; ++i) cin >> a[i];
	for(int i = n+1; i <= M; ++i) {
		a[i] = M*1e10;
		for(int j = 1; j <= n; ++j) a[i] = min(a[i], a[i-j]+a[j]);
	}

	int best = 1;
	for(int i = 2; i <= n; ++i) if(a[i]*best < a[best]*i) best = i;
	for(int i = 0; i < best; ++i) {
		b[i] = n+1;
		while(b[i]%best != i) ++b[i];
		for(int j = b[i]+best; j <= M; j += best) if(a[j]*b[i] < a[b[i]]*j) b[i] = j;
	}

	while(q--) {
		int k; cin >> k;
		if(k <= M) cout << a[k] << '\n';
		else {
			int j = b[k%best];
			cout << a[j]+a[best]*((k-j)/best) << '\n';
		}
	}

	return 0;
}