#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int T;
	cin >> T;
	while(T --> 0) {
		ll h, c, t;
		cin >> h >> c >> t;
		if(2*t <= c+h) cout << "2\n";
		else {
			ll res = 0;
			for(ll a = t; a > 0; a >>= 1) {
				while(((res+a+1)*h + (res+a)*c) >= t * (2*res+2*a+1)) res += a;
			}
			ll d0 = (res+1)*h + res*c - (2*res+1)*t;
			++ res;
			ll d1 = (2*res+1)*t - (res+1)*h - res*c;
			if(d0*(2*res+1) <= d1*(2*res-1)) cout << 2*res-1 << "\n";
			else cout << 2*res+1 << "\n";
		}
	}

	return 0;
}