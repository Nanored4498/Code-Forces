#include <iostream>

using namespace std;
const int N = 200;

int f(int x) {
	cout << "? " << N*((N+x-1)/x) << endl;
	for(int i = 1; i <= N; i += x)
		for(int j = 1; j <= N; ++j)
			cout << i << ' ' << j << ' ';
	cout << endl;
	int ans;
	cin >> ans;
	return ans;
}

int main() {
	int a[8], p = 4, q = 4;
	a[0] = f(1);
	for(int i = 1; i < 8; ++i) a[i] = a[0];
	while(q > 0) {
		a[p] = f(1<<p)*(1<<p);
		q >>= 1;
		if(a[p] == a[1]) p += q;
		else p -= q;
	}
	cout << "! ";
	for(int i = 1; i < 8; ++i) if(a[i] != a[0]) {
		int l = abs(a[0]-a[i]) / (1<<(i-1));
		cout << 2*(l+(a[0]/l))-4 << endl;
		return 0;
	}
	cout << 256+(a[0]/64)-4 << endl;
	return 0;
}