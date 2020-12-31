#include <iostream>

using namespace std;
const int MAX_N = 50;

int p[MAX_N];
bool seen[MAX_N+1];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t, n, a;
	cin >> t;
	while(t--) {
		cin >> n;
		for(int i = 1; i <= n; ++i) seen[i] = false;
		for(int i = 0; i < 2*n; ++i) {
			cin >> a;
			if(seen[a]) continue;
			seen[a] = true;
			cout << a << " ";
		}
		cout << "\n";
	}

	return 0;
}