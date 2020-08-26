#include <iostream>

using namespace std;
typedef long long ll;

int c[20];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	for(int i = 0; i < n; ++i) {
		int a;
		cin >> a;
		for(int j = 0; j < 20; ++j) if(a&(1<<j)) ++c[j];
	}
	ll ans = 0;
	for(int i = 0; i < n; ++i) {
		ll a = 0;
		for(int j = 0; j < 20; ++j) if(c[j]) {
			a += 1<<j;
			-- c[j];
		}
		ans += a*a;
	}
	cout << ans << "\n";

	return 0;
}