#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
const ll MOD = 998244353;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	if(m > n) return cout << "0\n", 0;
	vi a(n), b(m);
	for(int i = 0; i < n; ++i) cin >> a[i];
	for(int i = 0; i < m; ++i) cin >> b[i];
	vi last(m, -1);
	ll ans = 1;
	int j = m-1;
	for(int i = n-1; i >= 0; --i) {
		if(a[i] > b[j]) continue;
		if(a[i] == b[j] && last[j] == -1) last[j] = i;
		if(a[i] < b[j]) {
			if(last[j] < 0) return cout << "0\n", 0;
			--j;
			if(j < 0 || a[i] < b[j]) return cout << "0\n", 0;
			if(a[i] == b[j]) last[j] = i;
			ans = (ans * (last[j+1]-i)) % MOD;
		}
	}
	if(last[0] < 0) cout << "0\n";
	else cout << ans << "\n";

	return 0;
}