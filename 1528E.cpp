#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
const ll MOD = 998244353;
const ll iv2 = 499122177;
const ll iv6 = 166374059;

ll mul(ll a, ll b) { return (a*b) % MOD; }

int main() {
	int n;
	cin >> n;
	vector<ll> B(n);
	B[0] = 1;
	ll sB = 0;
	for(int i = 1; i < n; ++i) {
		B[i] = (mul(B[i-1], sB+1) + mul(B[i-1], mul(B[i-1]+1, iv2))) % MOD;
		sB = (sB + B[i-1]) % MOD;
	}
	ll ans = 0;

	// root with 3 children
	ans = (ans + mul(sB + B[n-1], B[n-1])) % MOD; // one child has depth n-1
	ans = (ans + mul(sB+1, mul(B[n-1], mul(B[n-1]+1, iv2)))) % MOD; // two child n-1
	ans = (ans + mul(B[n-1], mul(B[n-1]+1, mul(B[n-1]+2, iv6)))) % MOD; // three child n-1
	ans = (ans - 1 + MOD) % MOD; // remove chain
	ans = mul(ans, 2); // two possible orientation for edges

	// choose two internal nodes of the main chain and replace extremities with trees
	for(int i = 1; i < n-1; ++i) {
		ll left = (B[i] - B[i-1] + MOD) % MOD;
		ll right = (B[n-1-i] - 1 + MOD) % MOD;
		ans = (ans + mul(left, right)) % MOD;
	}

	ans = (ans + 1) % MOD; // add chain 
	cout << ans << endl;

	return 0;
}