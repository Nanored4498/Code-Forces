#include <iostream>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		string a, b;
		cin >> n >> a >> b;
		int c[4] = {};
		for(int i = 0; i < n; ++i)
			++ c[2*int(a[i]-'0')+int(b[i]-'0')];
		int ans = 1e9;
		if(c[1]==c[2]) ans = min(ans, 2*c[1]);
		if(c[3]==c[0]+1) ans = min(ans, 2*c[0]+1);
		if(ans == 1e9) ans = -1;
		cout << ans << '\n';
	}

	return 0;
}