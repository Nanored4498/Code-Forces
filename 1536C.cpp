#include <iostream>
#include <algorithm>
#include <map>

using namespace std;
typedef pair<int, int> pii;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		string s;
		cin >> s;
		map<pii, int> m;
		int D = 0, K = 0;
		for(char c : s) {
			if(c == 'D') ++ D;
			else ++ K;
			int g = __gcd(D, K);
			pii p = { D/g, K/g };
			++ m[p];
			cout << m[p] << ' ';
		}
		cout << '\n';
	}

	return 0;
}