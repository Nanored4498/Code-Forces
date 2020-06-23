#include <iostream>

using namespace std;
typedef long long ll;
const ll MOD = 1e9+7;
const int MAX = 2e6+5;
ll ans[MAX];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ans[1] = ans[2] = 0;
	ans[3] = ans[4] = 4;
	for(int i = 5; i < MAX; ++i) {
		if(i%3 == 0) ans[i] = (4+4*ans[i-4]+4*ans[i-3]+ans[i-2]) % MOD;
		else ans[i] = (2*ans[i-2]+ans[i-1]) % MOD;
	}
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		cout << ans[n] << "\n";
	}

	return 0;
}