#include <iostream>
#include <map>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	map<string, int> m;
	while(n--) {
		string s;
		cin >> s;
		++ m[s];
	}
	string ans = "";
	int best = 0;
	for(const auto &[s, x] : m) if(x > best) {
		best = x;
		ans = s;
	}
	cout << ans << endl;

	return 0;
}