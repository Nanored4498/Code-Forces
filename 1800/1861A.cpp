#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using vi = vector<int>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	while(t--) {
		string s;
		cin >> s;
		int i = 0;
		while(s[i] != '1' && s[i] != '3') ++i;
		if(s[i] == '1') cout << "13\n";
		else cout << "31\n";
	}

	return 0;
}