#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

vi a;

int f(int i, int j, int s) {
	if(j < i) return 0;
	int k = i;
	for(int l = i+1; l <= j; ++l) if(a[l] < a[k]) k = l;
	return min(j-i+1, a[k]-s + f(i, k-1, a[k]) + f(k+1, j, a[k]));
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	a.resize(n);
	for(int i = 0; i < n; ++i) cin >> a[i];
	cout << f(0, n-1, 0) << "\n";

	return 0;
}