#include <iostream>

using namespace std;
typedef long long ll;
const ll MOD = 998244353;
const ll MAX_n = 1e6+5;

ll nd[MAX_n], p[MAX_n];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	nd[1] = 1;
	ll sum = 1, ans = 1;
	for(int i = 2; i <= n; ++i) {
		if(p[i] == 0)
			for(int j = i; j <= n; j += i)
				p[j] = i;
		nd[i] = 1;
		int j = i;
		while(j % p[i] == 0) {
			++ nd[i];
			j /= p[i];
		}
		nd[i] *= nd[j];
		ans = (sum + nd[i]) % MOD;
		sum = (sum + ans) % MOD;
	}
	cout << ans << '\n';

	return 0;
}