#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<int> vi;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;
	vi a(N), b(N);
	for(int i = 0; i < N; ++i) cin >> a[i] >> b[i];
	int res = 0;
	for(int i = 0; i < N; ++i) {
		int n = 0;
		for(int add = N>>1; add > 0; add>>=1) {
			while(i-n-add >= 0 && a[i] >= a[i-n-add] && a[i]+n+add <= min(b[i], b[i-n-add]))
				n += add;
		}
		res = max(res, n+1);
		n = 0;
		for(int add = N>>1; add > 0; add>>=1) {
			while(i+n+add < N && a[i] >= a[i+n+add] && a[i]+n+add <= min(b[i], b[i+n+add]))
				n += add;
		}
		res = max(res, n+1);
	}
	cout << res << "\n";

	return 0;
}