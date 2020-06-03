#include <iostream> 
#include <vector>
#include <iomanip>

using namespace std;
typedef long double ld;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, k;
	cin >> n >> k;
	vector<ld> s(n);
	for(int i = 0; i < n; ++i) cin >> s[i];
	vector<ld> f(n);
	f[0] = ld(k)/ld(n);
	for(int j = 1; j <= n-k; ++j) {
		f[j] = f[j-1] * ld(n-j-k+1) / ld(n-j);
	}
	cout << fixed << setprecision(7);
	for(int i = 0; i < n; ++i) {
		double ans = 0;
		for(int j = 0; j <= n-k; ++j)
			ans += s[(i+n-j)%n] * f[j];
		cout << ans << " \n"[i+1 == n];
	}

	return 0;
}