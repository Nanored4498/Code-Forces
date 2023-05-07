#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, q;
	string s;
	cin >> n >> s >> q;
	int p = 0;
	for(char c : s) if(c == '+') ++p;
	int m = n-p;
	while(q--) {
		ll a, b; cin >> a >> b;
		ll x = (p-m) * a;
		ll d = b-a;
		if(d == 0) {
			cout << (x == 0 ? "YES" : "NO") << '\n';
		} else {
			ll q = x/d;
			if(x != q*d) cout << "NO\n";
			else if(q < 0) cout << (-q <= p ? "YES" : "NO") << '\n';
			else cout << (q <= m ? "YES" : "NO") << '\n';
		}
	}

	return 0;
}