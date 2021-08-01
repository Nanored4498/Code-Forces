#include <iostream>

using namespace std;

int n;
int a[10], c[10];

bool f(int i, int s) {
	if(i == -1) {
		if(s != 0) return false;
		for(i = 0; i < n; ++i) if(c[i] != 0) return true;;
		return false;
	}
	c[i] = 0;  if(f(i-1, s)) return true;
	c[i] = 1;  if(f(i-1, s+a[i])) return true;
	c[i] = -1; if(f(i-1, s-a[i])) return true;
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		cin >> n;
		for(int i = 0; i < n; ++i) cin >> a[i];
		cout << (f(n-1, 0) ? "YES" : "NO") << endl;
	}

	return 0;
}