#include <iostream>

using namespace std;

long long x[4][3] = {{0, 0, 0}, {1, 2, 3}, {2, 3, 1}, {3, 1, 2}};

int main() {
	ios::sync_with_stdio(false);
	cout.tie(nullptr);

	int t;
	cin >> t;
	while(t --> 0) {
		long long n;
		cin >> n;
		n -= 1;
		long long m3 = n % 3;
		n /= 3;
		long long mul = 1;
		long long res = 0;
		while(n > 0) {
			n -= 1;
			long long a = n % 4;
			res += mul * x[a][m3];
			mul *= 4;
			n /= 4;
		}
		res += mul * (m3+1);
		cout << res << "\n";
	}

	return 0;
}