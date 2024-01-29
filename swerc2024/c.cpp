#include <iostream>
#include <algorithm>
#include <bit>
#include <iomanip>

using namespace std;
using ll = unsigned long long;

int n, m;
int s[50];
ll t[20];
float mem[387'420'489];

float f(int x, ll y) {
	if(!mem[x]) {
		const int k = popcount(y);
		if(k == 1) return 0.f;
		int x2 = x, mul = 1;
		float ans = 1e9;
		for(int i = 0; i < n; ++i) {
			if(!(x2%3)) {
				const ll y1 = y & t[i];
				const int k1 = popcount(y1);
				if(k1 != k && k1 != 0) {
					ans = min(ans, 1.f + (k1 * f(x + mul, y1) + (k-k1) * f(x + 2*mul, y^y1)) / k);
				}
			}
			x2 /= 3;
			mul *= 3;
		}
		mem[x] = ans;
	}
	return mem[x];
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
	cout << fixed << setprecision(5) << f(0, (1LL<<m)-1) << '\n';

	return 0;
}