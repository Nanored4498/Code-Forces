#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vi h(n);
	for(int &x : h) cin >> x;
	ll num = 0, den = 1;
	vi st;
	for(int i = 1; i < n; ++i) {
		if(h[i] > h[i-1]) {
			while(!st.empty()) {
				int j = st.back();
				if(h[j] < h[i]) {
					ll d = h[i] - h[i-1];
					ll n = d*(i-1-j) + h[j] - h[i-1];
					ll g = gcd(d, n);
					d /= g;
					n /= g;
					if(num*d < n*den) {
						num = n;
						den = d;
					}
					st.pop_back();
				} else {
					ll d = h[j] - h[j+1];
					ll n = d*(i-1-j) + h[i] - h[j+1];
					ll g = gcd(d, n);
					d /= g;
					n /= g;
					if(num*d < n*den) {
						num = n;
						den = d;
					}
					break;
				}
			}
		} else {
			st.push_back(i-1);
		}
	}
	cout << num;
	if(den != 1) cout << '/' << den;
	cout << '\n';

	return 0;
}