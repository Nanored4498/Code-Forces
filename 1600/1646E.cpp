#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
	int n, m;
	cin >> n >> m;

	const int M = 20;
	vector<bool> seen(M*m+1, false);
	vector<int> count(M+1, 0);
	for(int i = 1; i <= M; ++i) {
		count[i] = count[i-1];
		for(int j = 1; j <= m; ++j) if(!seen[i*j]) {
			seen[i*j] = true;
			++ count[i];
		}
	}

	ll ans = 1;
	seen.assign(n+1, false);
	for(int i = 2; i <= n; ++i) if(!seen[i]) {
		int mp = 1, ip = i;
		while((ll) ip*i <= n) {
			++ mp;
			ip *= i;
			seen[ip] = true;
		}
		ans += count[mp];
	}
	cout << ans << endl;

	return 0;
}