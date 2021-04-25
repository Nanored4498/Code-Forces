#include <iostream>

using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int T;
	cin >> T;
	while(T--) {
		ll n;
		cin >> n;
		if(n % 2050LL == 0LL) {
			n /= 2050LL;
			int ans = 0;
			while(n > 0) {
				ans += n % 10LL;
				n /= 10LL;
			}
			cout << ans << '\n';
		} else cout << "-1\n";
	}

	return 0;
}