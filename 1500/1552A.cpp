#include <iostream>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		string s;
		cin >> n >> s;
		string ss = s;
		sort(ss.begin(), ss.end());
		int k = 0;
		for(int i = 0; i < n; ++i) if(ss[i] != s[i]) ++ k;
		cout << k << '\n';
	}

	return 0;
}