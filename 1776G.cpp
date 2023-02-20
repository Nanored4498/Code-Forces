#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	string s;
	cin >> n >> s;
	int x = 0;
	for(int i = 0; i < n; ++i) if(s[i] == 'W') ++x;
	int cur = x;
	for(int i = 0; i < n-1; ++i) {
		if(s[i] == 'W') -- cur;
		if(s[i+n] == 'W') ++ cur;
		x = max(x, cur);
	}
	cout << x << '\n';

	return 0;
}