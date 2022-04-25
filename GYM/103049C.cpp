#include <iostream>
#include <iomanip>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << setprecision(9);

	int n, k; cin >> n >> k;
	double d, s; cin >> d >> s;
	double a = (d*n - s*k)/(n-k);
	if(a >= 0. && a <= 100.) cout << a << endl;
	else cout << "impossible\n";

	return 0;
}