#include <iostream>
#include <algorithm>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll r;
	int n;
	cin >> r >> n;
	ll a = 0, b = r;
	while(n--) {
		ll d;
		cin >> d;
		a = clamp(a+d, 0LL, r);
		b = clamp(b+d, 0LL, r);
	}
	if(a == b) cout << a << endl;
	else cout << "uncertain\n";

	return 0;
}