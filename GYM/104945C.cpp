#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <bit>
#include <iomanip>

using namespace std;
using ll = unsigned long long;

int n, m;
int s[50];
ll t[20];
unordered_map<ll, float> mem;

float f(ll y) {
	if(!mem.count(y)) {
		const int k = popcount(y);
		if(k == 1) return 0.f;
		float ans = 1e9;
		for(int i = 0; i < n; ++i) {
			const ll y1 = y & t[i];
			const int k1 = popcount(y1);
			if(k1 != k && k1 != 0) {
				ans = min(ans, 1.f + (k1 * f(y1) + (k-k1) * f(y^y1)) / k);
			}
		}
		mem[y] = ans;
	}
	return mem[y];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	for(int i = 0; i < m; ++i) {
		int k; cin >> k;
		while(k--) {
			int j; cin >> j;
			s[i] |= (1<<j);
			t[j] |= (1LL<<i);
		}
	}
	sort(s, s+m);
	for(int i = 1; i < m; ++i) {
		if(s[i] == s[i-1]) {
			cout << "not possible\n";
			return 0;
		}
	}
	cout << fixed << setprecision(5) << f((1LL<<m)-1) << '\n';

	return 0;
}