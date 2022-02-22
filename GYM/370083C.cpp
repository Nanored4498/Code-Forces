#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
const ll MOD = 1e9+7;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	ll a = 1, b = 1;
	while(n--) {
		a = (a*27) % MOD;
		b = (b*7) % MOD;
	}
	cout << ((a+MOD-b) % MOD) << endl;

	return 0;
}